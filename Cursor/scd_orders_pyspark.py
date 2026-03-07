"""
PySpark implementation of SCD (Slowly Changing Dimension) types for Orders table.

SCD Type 1: Overwrite - no history, current state only
SCD Type 2: Full history - new row per change with effective dates & version
SCD Type 3: Previous value - current + previous value in columns
SCD Type 4: History table - current table + separate history table
"""

from functools import reduce

from pyspark.sql import SparkSession
from pyspark.sql import functions as F
from pyspark.sql.types import (
    StructType,
    StructField,
    StringType,
    DoubleType,
    TimestampType,
    IntegerType,
)


def get_orders_schema():
    """Schema for orders dimension table."""
    return StructType([
        StructField("order_id", StringType(), False),
        StructField("customer_id", StringType(), True),
        StructField("order_status", StringType(), True),
        StructField("total_amount", DoubleType(), True),
        StructField("region", StringType(), True),
        StructField("updated_at", TimestampType(), True),
    ])


# ---------------------------------------------------------------------------
# SCD Type 1: Overwrite - no history kept
# ---------------------------------------------------------------------------


def scd_type1(target_df, source_df, key_column="order_id"):
    """
    SCD Type 1: Overwrite existing records with new values. No history kept.

    - Matching keys: overwrite with source values
    - New keys: insert
    - Keys only in target: retain (or remove if you want source-only)
    """
    # Records in source (updates + inserts)
    source_keys = source_df.select(key_column).distinct()
    # Existing in target but not in source - keep as-is (or drop for source-only)
    target_only = target_df.join(source_keys, key_column, "left_anti")
    # All from source (overwrites existing + new)
    from_source = source_df
    return target_only.unionByName(from_source, allowMissingColumns=True).dropDuplicates([key_column])


def scd_type1_delta_merge(spark, target_path, source_df, key_column="order_id"):
    """
    SCD Type 1 using Delta Lake MERGE.
    Overwrites on match, inserts on no match. Use when target is a Delta table.
    """
    from delta.tables import DeltaTable

    target = DeltaTable.forPath(spark, target_path)
    target.alias("target").merge(
        source_df.alias("source"),
        f"target.{key_column} = source.{key_column}"
    ).whenMatchedUpdateAll().whenNotMatchedInsertAll().execute()


# ---------------------------------------------------------------------------
# SCD Type 2: Full history - new row per change
# ---------------------------------------------------------------------------


def scd_type2(
    target_df,
    source_df,
    key_column="order_id",
    version_column="version",
    effective_from="effective_from",
    effective_to="effective_to",
    is_current="is_current",
    change_columns=None,
):
    """
    SCD Type 2: Full history. New row for each change with version and effective dates.

    - No change: keep as-is
    - Changed: close current row (effective_to=now, is_current=false), insert new row
    - New: insert with version=1, is_current=true
    """
    if change_columns is None:
        change_columns = [c for c in source_df.columns if c != key_column]

    # Ensure target has SCD columns
    for col_name in [version_column, effective_from, effective_to, is_current]:
        if col_name not in target_df.columns:
            if col_name == version_column:
                target_df = target_df.withColumn(col_name, F.lit(1).cast(IntegerType()))
            elif col_name == effective_from:
                target_df = target_df.withColumn(col_name, F.current_timestamp())
            elif col_name == effective_to:
                target_df = target_df.withColumn(col_name, F.lit(None).cast(TimestampType()))
            elif col_name == is_current:
                target_df = target_df.withColumn(col_name, F.lit(True))

    current = target_df.filter(F.col(is_current) == True)
    historical = target_df.filter(F.col(is_current) == False) if is_current in target_df.columns else target_df.limit(0)

    # New records (in source, not in current target)
    new_records = source_df.join(current.select(key_column), key_column, "left_anti")
    new_records = new_records.withColumn(version_column, F.lit(1)) \
        .withColumn(effective_from, F.current_timestamp()) \
        .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
        .withColumn(is_current, F.lit(True))

    # Join source with current
    combined = source_df.alias("source").join(current.alias("target"), key_column, "full_outer")

    # Detect changes
    change_conds = []
    for c in change_columns:
        if c in source_df.columns and c in current.columns:
            change_conds.append(
                F.coalesce(
                    F.col(f"source.{c}") != F.col(f"target.{c}"),
                    F.col(f"source.{c}").isNull() != F.col(f"target.{c}").isNull(),
                    F.lit(True)
                ) & F.col(f"target.{key_column}").isNotNull()
            )
    has_change = reduce(lambda a, b: a | b, change_conds) if change_conds else F.lit(False)

    # Unchanged
    unchanged = combined.filter(~has_change & F.col(f"target.{key_column}").isNotNull()).select(
        [F.col(f"target.{c}").alias(c) for c in target_df.columns]
    )

    # Changed: close old row + add new row
    changed_pairs = combined.filter(has_change)
    closed = changed_pairs.select(
        [F.col(f"target.{c}").alias(c) for c in target_df.columns]
    ).withColumn(effective_to, F.current_timestamp()).withColumn(is_current, F.lit(False))

    new_versions = changed_pairs.select(
        [F.col(f"source.{c}").alias(c) for c in source_df.columns]
    ).withColumn(version_column, F.col(f"target.{version_column}") + 1) \
        .withColumn(effective_from, F.current_timestamp()) \
        .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
        .withColumn(is_current, F.lit(True))

    return (
        historical.unionByName(unchanged, allowMissingColumns=True)
        .unionByName(closed, allowMissingColumns=True)
        .unionByName(new_versions, allowMissingColumns=True)
        .unionByName(new_records, allowMissingColumns=True)
    )


def scd_type2_hash(
    target_df,
    source_df,
    key_column="order_id",
    version_column="version",
    effective_from="effective_from",
    effective_to="effective_to",
    is_current="is_current",
    value_columns=None,
):
    """
    SCD Type 2 using hash comparison. Simpler and handles all value columns.
    """
    if value_columns is None:
        value_columns = [c for c in source_df.columns if c not in (key_column, version_column, effective_from, effective_to, is_current)]

    def add_hash(df, cols):
        return df.withColumn("_hash", F.sha2(F.concat_ws("|", *[F.coalesce(F.col(c).cast("string"), F.lit("")).alias(c) for c in cols if c in df.columns]), 256))

    target_with_hash = add_hash(target_df, value_columns)
    source_with_hash = add_hash(source_df, value_columns)

    current = target_with_hash.filter(F.col(is_current) == True) if is_current in target_with_hash.columns else target_with_hash
    if is_current not in target_with_hash.columns:
        current = current.withColumn(is_current, F.lit(True)) \
            .withColumn(version_column, F.lit(1)) \
            .withColumn(effective_from, F.current_timestamp()) \
            .withColumn(effective_to, F.lit(None).cast(TimestampType()))

    historical = target_with_hash.filter(F.col(is_current) == False) if is_current in target_with_hash.columns else target_with_hash.limit(0)

    joined = source_with_hash.alias("s").join(current.alias("t"), key_column, "full_outer")

    # New records
    new = source_df.join(current.select(key_column), key_column, "left_anti") \
        .withColumn(version_column, F.lit(1)) \
        .withColumn(effective_from, F.current_timestamp()) \
        .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
        .withColumn(is_current, F.lit(True))

    # Changed: both exist, hash different
    changed = joined.filter(
        F.col("s._hash").isNotNull() & F.col("t._hash").isNotNull() &
        (F.col("s._hash") != F.col("t._hash"))
    )

    closed = changed.select([F.col(f"t.{c}").alias(c) for c in current.columns if c != "_hash"]) \
        .withColumn(effective_to, F.current_timestamp()).withColumn(is_current, F.lit(False))

    opened = changed.select([F.col(f"s.{c}").alias(c) for c in source_df.columns]) \
        .withColumn(version_column, F.col("t.version") + 1) \
        .withColumn(effective_from, F.current_timestamp()) \
        .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
        .withColumn(is_current, F.lit(True))

    # Unchanged
    unchanged = joined.filter(
        F.col("s._hash").isNotNull() & F.col("t._hash").isNotNull() &
        (F.col("s._hash") == F.col("t._hash"))
    ).select([F.col(f"t.{c}").alias(c) for c in current.columns if c != "_hash"])

    historical_clean = historical.drop("_hash") if "_hash" in historical.columns else historical

    return (
        historical_clean.unionByName(unchanged, allowMissingColumns=True)
        .unionByName(closed, allowMissingColumns=True)
        .unionByName(opened, allowMissingColumns=True)
        .unionByName(new, allowMissingColumns=True)
    )


# ---------------------------------------------------------------------------
# SCD Type 3: Previous value in same row
# ---------------------------------------------------------------------------


def scd_type3(
    target_df,
    source_df,
    key_column="order_id",
    prev_suffix="_prev",
    track_columns=None,
):
    """
    SCD Type 3: Keep current + previous value in same row.

    - Adds {col}_prev for each tracked column
    - On change: current -> _prev, new -> current
    - New records: current only, _prev is null
    """
    if track_columns is None:
        track_columns = [c for c in source_df.columns if c != key_column]

    for c in track_columns:
        prev_col = f"{c}{prev_suffix}"
        if prev_col not in target_df.columns:
            target_df = target_df.withColumn(prev_col, F.lit(None).cast(StringType()))

    joined = source_df.alias("s").join(target_df.alias("t"), key_column, "full_outer")

    select_exprs = [
        F.coalesce(F.col(f"s.{key_column}"), F.col(f"t.{key_column}")).alias(key_column)
    ]
    for c in track_columns:
        if c not in source_df.columns:
            continue
        prev_col = f"{c}{prev_suffix}"
        changed = F.coalesce(
            F.col(f"s.{c}") != F.col(f"t.{c}"),
            F.col(f"s.{c}").isNull() != F.col(f"t.{c}").isNull(),
            F.lit(False)
        ) & F.col(f"t.{key_column}").isNotNull()
        select_exprs.append(
            F.when(F.col(f"s.{key_column}").isNotNull(), F.col(f"s.{c}"))
            .otherwise(F.col(f"t.{c}"))
            .alias(c)
        )
        select_exprs.append(
            F.when(changed, F.col(f"t.{c}"))
            .otherwise(F.col(f"t.{prev_col}"))
            .alias(prev_col)
        )

    return joined.select(select_exprs).dropDuplicates([key_column])


# ---------------------------------------------------------------------------
# SCD Type 4: Current + History table
# ---------------------------------------------------------------------------


def scd_type4(
    target_current_df,
    target_history_df,
    source_df,
    key_column="order_id",
    history_columns=None,
):
    """
    SCD Type 4: Current table (Type 1) + separate history table (append-only).

    - Current: SCD Type 1 behavior (overwrite)
    - History: append closed/changed rows on each update
    """
    if history_columns is None:
        history_columns = [c for c in source_df.columns if c != key_column]

    # Current: SCD Type 1
    new_current = scd_type1(target_current_df, source_df, key_column)

    # History: rows that were in target_current but changed or removed
    target_alias = target_current_df.alias("target_current")
    source_alias = source_df.alias("s")
    change_conds = [
        F.col(f"target_current.{c}") != F.col(f"s.{c}")
        for c in history_columns
        if c in target_current_df.columns and c in source_df.columns
    ]
    changed = target_alias.join(source_alias, key_column, "inner").filter(
        reduce(lambda a, b: a | b, change_conds, F.lit(False))
    ).select("target_current.*")

    changed_or_removed = target_current_df.join(
        source_df.select(key_column),
        key_column,
        "left_anti"
    ).union(changed).dropDuplicates([key_column])

    # Add history metadata
    history_new = changed_or_removed.withColumn("history_effective_to", F.current_timestamp())

    new_history = target_history_df.unionByName(history_new, allowMissingColumns=True)

    return new_current, new_history


# ---------------------------------------------------------------------------
# Delta Lake SCD Type 2 (production)
# ---------------------------------------------------------------------------


def scd_type2_delta(spark, target_path, source_df, key_column="order_id",
                    version_column="version", effective_from="effective_from",
                    effective_to="effective_to", is_current="is_current"):
    """
    Production SCD Type 2 using Delta Lake merge.
    Target table must have columns: key + value cols + version, effective_from, effective_to, is_current.
    """
    from delta.tables import DeltaTable
    import os

    value_cols = [c for c in source_df.columns if c != key_column]
    now = F.current_timestamp()

    # Prepare source with SCD columns
    source_prepared = source_df.withColumn("_effective_from", now)

    if not os.path.exists(target_path) or not DeltaTable.isDeltaTable(spark, target_path):
        # Initial load
        initial = source_prepared \
            .withColumn(version_column, F.lit(1)) \
            .withColumn(effective_from, F.col("_effective_from")) \
            .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
            .withColumn(is_current, F.lit(True)) \
            .drop("_effective_from")
        initial.write.format("delta").mode("overwrite").save(target_path)
        return

    target = DeltaTable.forPath(spark, target_path)
    target_df = target.toDF()
    current = target_df.filter(F.col(is_current) == True)

    # New records
    new_keys = source_prepared.join(current.select(key_column), key_column, "left_anti")
    new_records = new_keys.withColumn(version_column, F.lit(1)) \
        .withColumn(effective_from, F.col("_effective_from")) \
        .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
        .withColumn(is_current, F.lit(True)) \
        .drop("_effective_from")

    # Changed records: join and compare
    matched = source_prepared.alias("s").join(current.alias("t"), key_column, "inner")
    change_expr = F.lit(False)
    for c in value_cols:
        if c in current.columns:
            change_expr = change_expr | (
                F.coalesce(F.col("s." + c) != F.col("t." + c), F.col("s." + c).isNull() != F.col("t." + c).isNull())
            )
    changed = matched.filter(change_expr)

    # Close old rows (update is_current, effective_to)
    close_updates = {is_current: F.lit(False), effective_to: now}
    keys_to_close = changed.select(key_column)

    target.alias("t").merge(
        keys_to_close.alias("s"),
        f"t.{key_column} = s.{key_column} AND t.{is_current} = true"
    ).whenMatchedUpdate(set=close_updates).execute()

    # Insert new versions and new records
    if changed.count() > 0:
        max_versions = target.toDF().groupBy(key_column).agg(F.max(version_column).alias("_max_v"))
        new_versions = changed.select([F.col("s." + c).alias(c) for c in source_prepared.columns]) \
            .join(max_versions, key_column, "left") \
            .withColumn(version_column, F.coalesce(F.col("_max_v"), F.lit(0)) + 1) \
            .withColumn(effective_from, now) \
            .withColumn(effective_to, F.lit(None).cast(TimestampType())) \
            .withColumn(is_current, F.lit(True)) \
            .drop("_effective_from", "_max_v")
        new_records = new_records.unionByName(new_versions, allowMissingColumns=True)

    if new_records.count() > 0:
        new_records.write.format("delta").mode("append").save(target_path)


# ---------------------------------------------------------------------------
# Sample data and demo
# ---------------------------------------------------------------------------


def create_sample_orders(spark):
    """Create sample orders target and source for testing."""
    schema = get_orders_schema()
    target = spark.createDataFrame([
        ("ord1", "cust1", "SHIPPED", 100.0, "West", None),
        ("ord2", "cust2", "PENDING", 50.0, "East", None),
    ], schema)
    target = target \
        .withColumn("version", F.lit(1)) \
        .withColumn("effective_from", F.current_timestamp()) \
        .withColumn("effective_to", F.lit(None).cast(TimestampType())) \
        .withColumn("is_current", F.lit(True))

    source = spark.createDataFrame([
        ("ord1", "cust1", "DELIVERED", 100.0, "West", None),   # status changed
        ("ord2", "cust2", "PENDING", 75.0, "East", None),      # amount changed
        ("ord3", "cust3", "NEW", 200.0, "North", None),        # new order
    ], schema)

    return target, source


if __name__ == "__main__":
    spark = (
        SparkSession.builder
        .appName("SCD_Orders")
        .config("spark.sql.extensions", "io.delta.sql.DeltaSparkSessionExtension")
        .config("spark.sql.catalog.spark_catalog", "org.apache.spark.sql.delta.DeltaCatalog")
        .getOrCreate()
    )
    target, source = create_sample_orders(spark)

    print("=== TARGET (before) ===")
    target.show(truncate=False)

    print("=== SOURCE (incoming) ===")
    source.show(truncate=False)

    print("\n=== SCD Type 1 (Overwrite) ===")
    scd_type1(target.drop("version", "effective_from", "effective_to", "is_current"), source).show(truncate=False)

    print("\n=== SCD Type 2 Hash (Full history) ===")
    result_t2 = scd_type2_hash(
        target, source,
        key_column="order_id",
        version_column="version",
        effective_from="effective_from",
        effective_to="effective_to",
        is_current="is_current",
    )
    result_t2.orderBy("order_id", "effective_from").show(truncate=False)

    print("\n=== SCD Type 3 (Previous value) ===")
    target_t3 = target.select("order_id", "customer_id", "order_status", "total_amount", "region", "updated_at")
    result_t3 = scd_type3(target_t3, source, key_column="order_id", track_columns=["order_status", "total_amount", "region"])
    result_t3.show(truncate=False)

    print("\n=== SCD Type 4 (Current + History) ===")
    target_cur = target.select("order_id", "customer_id", "order_status", "total_amount", "region", "updated_at")
    target_hist = spark.createDataFrame([], target_cur.schema.add("history_effective_to", TimestampType(), True))
    cur_4, hist_4 = scd_type4(target_cur, target_hist, source)
    print("Current:")
    cur_4.show(truncate=False)
    print("History:")
    hist_4.show(truncate=False)
