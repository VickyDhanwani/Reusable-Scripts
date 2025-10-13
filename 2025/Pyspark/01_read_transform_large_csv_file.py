from pyspark.sql import SparkSession

#The entry point to programming Spark with the Dataset and DataFrame API.
# https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/spark_session.html

spark = SparkSession.builder.appName("CSVReader").config("spark.sql.adaptive.enabled", "true") \
        .config("spark.sql.adaptive.coleasePartitions.enabled", "true").getOrCreate() 
# https://spark.apache.org/docs/latest/sql-performance-tuning.html#adaptive-query-execution

"""
Adaptive Query Execution (AQE) - dynamically optimizes query plans at runtime
Benefits:

Automatically coalesces small partitions
Dynamically switches join strategies
Optimizes skew joins

What: Combines small shuffle partitions into larger ones to reduce overhead
Why: Prevents too many small files/tasks which hurt performance
Correct: spark.sql.adaptive.coalescePartitions.enabled
Docs: https://spark.apache.org/docs/latest/sql-performance-tuning.html#coalescing-post-shuffle-partitions

Key Configs:

spark.sql.shuffle.partitions: Number of partitions after shuffle (default 200)
spark.executor.memory: Memory per executor (default 1g)
spark.executor.cores: CPU cores per executor (default 1)
spark.sql.autoBroadcastJoinThreshold: Max size for broadcast join (default 10MB)

"""

#What: DataFrameReader - interface to load data into DataFrame
#Docs: https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.DataFrameReader.html

# ✅ GOOD: Define schema explicitly
from pyspark.sql.types import StructType, StructField, StringType, IntegerType, DateType

schema = StructType([
    StructField("id", IntegerType(), False),  # False = not nullable
    StructField("name", StringType(), True),
    StructField("order_date", DateType(), True),
    StructField("amount", IntegerType(), True)
])

df = spark.read \
     .option("header", "true") \
    .option("inferSchema", "true") \
    .option("mode", "DROPMALFORMED") \
    .schema(schema) \
    .csv("s3://bucket/input/large_file.csv")

# https://spark.apache.org/docs/latest/sql-data-sources-csv.html
# https://spark.apache.org/docs/latest/sql-data-sources-csv.html#data-source-option



df_transformed = df.filter(col("status").isNotNull()).withColumn("processed_date", current_timestamp()) \
                 .withColumn("year", to_date(col("order_date")).substr(1, 4)) \
                 .withColumn("month", to_date(col("order_date")).substr(6, 2))

# https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.DataFrame.filter.html
# https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.functions.current_timestamp.html


df_transformed.write.mode("overwrite").partitionBy("year", "month").parquet("s3://bucket/output/processed.csv")

# https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.DataFrameWriter.mode.html
# https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.DataFrameWriter.partitionBy.html
# https://spark.apache.org/docs/latest/sql-data-sources-parquet.html


spark.stop()


