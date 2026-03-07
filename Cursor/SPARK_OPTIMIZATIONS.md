# Spark Optimization Guide

A comprehensive guide to common Spark optimizations that should be applied while coding to improve performance, reduce resource consumption, and ensure efficient data processing.

## Table of Contents

1. [Partitioning Strategies](#partitioning-strategies)
2. [Caching and Persistence](#caching-and-persistence)
3. [Broadcast Variables](#broadcast-variables)
4. [Join Optimizations](#join-optimizations)
5. [Shuffle Optimizations](#shuffle-optimizations)
6. [Data Skew Handling](#data-skew-handling)
7. [Resource Management](#resource-management)
8. [Code-Level Optimizations](#code-level-optimizations)
9. [Data Serialization](#data-serialization)
10. [File Format Optimizations](#file-format-optimizations)
11. [Query Optimization](#query-optimization)
12. [Memory Management](#memory-management)

---

## Partitioning Strategies

### 1. Optimal Partition Count
- **Rule**: Aim for 2-3x the number of CPU cores per executor
- **Default**: `spark.default.parallelism` should be set appropriately
- **Calculation**: `numPartitions = totalCores * 2` or `numPartitions = totalDataSize / blockSize`
- **Best Practice**: Avoid too many small partitions (< 100MB) or too few large partitions (> 2GB)

```python
# Good: Explicit partitioning
df.repartition(200)  # Based on cluster size

# Better: Partition by key column
df.repartition("date", "region")

# Best: Coalesce to reduce partitions (no shuffle)
df.coalesce(50)  # Reduces partitions without full shuffle
```

### 2. Partition by Key Columns
- Partition data by columns frequently used in joins, filters, or aggregations
- Ensures data locality and reduces shuffles
- Use `partitionBy()` when writing to storage

```python
# Partition by date for time-series queries
df.write.partitionBy("date", "hour").parquet("output_path")

# Repartition before joins on specific keys
df1.repartition("customer_id")
df2.repartition("customer_id")
```

### 3. Avoid Unnecessary Repartitions
- Use `coalesce()` instead of `repartition()` when reducing partitions
- `coalesce()` avoids full shuffle, `repartition()` always shuffles
- Only repartition when absolutely necessary

---

## Caching and Persistence

### 1. Cache Frequently Used DataFrames
- Cache DataFrames that are:
  - Used multiple times in the same job
  - Small enough to fit in memory
  - Expensive to recompute

```python
# Cache when reused multiple times
df_filtered = df.filter(df.status == "active")
df_filtered.cache()  # or .persist(StorageLevel.MEMORY_AND_DISK)

# Use cached DataFrame multiple times
result1 = df_filtered.groupBy("category").count()
result2 = df_filtered.groupBy("region").count()

# Unpersist when done
df_filtered.unpersist()
```

### 2. Choose Appropriate Storage Levels
- `MEMORY_ONLY`: Fastest, but requires data to fit in memory
- `MEMORY_AND_DISK`: Spills to disk if memory is full (recommended default)
- `MEMORY_ONLY_SER`: Serialized, uses less memory
- `DISK_ONLY`: Slowest, use when memory is constrained

```python
from pyspark import StorageLevel

# For large datasets
df.persist(StorageLevel.MEMORY_AND_DISK)

# For memory-constrained environments
df.persist(StorageLevel.MEMORY_ONLY_SER)
```

### 3. Avoid Caching Large Datasets
- Don't cache datasets larger than available memory
- Monitor cache hit rates
- Unpersist unused cached DataFrames

---

## Broadcast Variables

### 1. Broadcast Small Lookup Tables
- Use broadcast joins for small tables (< 100MB)
- Automatically handled by Spark when one side is < 10MB
- Manually broadcast larger tables up to 8GB

```python
# Automatic broadcast (table < 10MB)
result = df1.join(df2, "key")

# Manual broadcast for larger tables
from pyspark.sql.functions import broadcast

small_df = spark.read.parquet("small_lookup_table")
result = df1.join(broadcast(small_df), "key")
```

### 2. Broadcast Join Hint
- Use `broadcast()` hint to force broadcast join
- Overrides Spark's automatic decision
- Useful when Spark underestimates table size

```python
# Force broadcast join
df1.join(broadcast(df2), df1.id == df2.id)
```

---

## Join Optimizations

### 1. Prefer Broadcast Joins
- Use broadcast joins for small tables
- Reduces shuffle overhead significantly
- Check join size before execution

### 2. Sort-Merge Join Optimization
- Ensure both sides are partitioned by join key
- Pre-sort data when possible
- Use `spark.sql.join.preferSortMergeJoin = true`

```python
# Partition both DataFrames by join key
df1 = df1.repartition("join_key")
df2 = df2.repartition("join_key")

# Then join
result = df1.join(df2, "join_key")
```

### 3. Bucket Joins
- Use bucketing for frequently joined tables
- Pre-partitions data by join key
- Enables bucket joins without shuffle

```python
# Write bucketed table
df.write.bucketBy(200, "customer_id").saveAsTable("bucketed_table")

# Join bucketed tables (no shuffle)
df1.join(df2, "customer_id")
```

### 4. Avoid Cartesian Joins
- Always specify join conditions
- Use explicit join keys
- Monitor for cartesian product warnings

---

## Shuffle Optimizations

### 1. Minimize Shuffle Operations
- Reduce number of shuffles by combining operations
- Use `coalesce()` instead of `repartition()` when possible
- Cache intermediate results to avoid recomputation

### 2. Optimize Shuffle Partitions
- Set `spark.sql.shuffle.partitions` appropriately (default: 200)
- Too many partitions: overhead from task scheduling
- Too few partitions: large partitions causing OOM

```python
# Set shuffle partitions based on data size
spark.conf.set("spark.sql.shuffle.partitions", "400")
```

### 3. Use Map-Side Aggregations
- Prefer `reduceByKey()` over `groupByKey()`
- Reduces shuffle data volume
- Combines values before shuffle

```python
# Good: Map-side aggregation
rdd.reduceByKey(lambda a, b: a + b)

# Avoid: Shuffles all data
rdd.groupByKey().mapValues(sum)
```

---

## Data Skew Handling

### 1. Identify Skewed Keys
- Monitor task execution times
- Look for tasks taking significantly longer
- Use Spark UI to identify skew

### 2. Salt Technique
- Add random salt to skewed keys
- Distributes load across partitions
- Remove salt after aggregation

```python
from pyspark.sql.functions import col, concat, lit, rand

# Add salt to skewed key
df_salted = df.withColumn("salted_key", 
    concat(col("skewed_key"), lit("_"), (rand() * 100).cast("int")))

# Aggregate with salted key
result = df_salted.groupBy("salted_key").agg(...)

# Remove salt and aggregate again
final_result = result.groupBy("skewed_key").agg(...)
```

### 3. Broadcast Skewed Keys
- For highly skewed joins, broadcast the smaller side
- Reduces shuffle for skewed partitions

### 4. Filter Early
- Filter data before joins/aggregations
- Reduces data volume early in pipeline
- Especially important for skewed data

---

## Resource Management

### 1. Configure Executor Resources
- Set appropriate executor memory and cores
- Balance between memory and parallelism
- Leave 10-20% memory for overhead

```python
# Example configuration
spark.conf.set("spark.executor.memory", "8g")
spark.conf.set("spark.executor.cores", "4")
spark.conf.set("spark.executor.memoryFraction", "0.8")
```

### 2. Dynamic Allocation
- Enable dynamic executor allocation
- Scales executors based on workload
- Reduces resource waste

```python
spark.conf.set("spark.dynamicAllocation.enabled", "true")
spark.conf.set("spark.dynamicAllocation.minExecutors", "2")
spark.conf.set("spark.dynamicAllocation.maxExecutors", "20")
```

### 3. Speculative Execution
- Enable for straggler tasks
- Helps with data skew and slow nodes
- May increase cluster load

```python
spark.conf.set("spark.speculation", "true")
spark.conf.set("spark.speculation.multiplier", "1.5")
```

---

## Code-Level Optimizations

### 1. Use Column Expressions Over UDFs
- Column expressions are optimized by Catalyst
- UDFs cannot be optimized and are slower
- Use built-in functions when possible

```python
# Good: Column expression
from pyspark.sql.functions import col, when
df.withColumn("category", when(col("age") < 18, "minor").otherwise("adult"))

# Avoid: UDF (when possible)
from pyspark.sql.functions import udf
@udf
def categorize(age):
    return "minor" if age < 18 else "adult"
df.withColumn("category", categorize(col("age")))
```

### 2. Avoid Collecting Large Datasets
- `collect()` brings all data to driver
- Use `take()`, `head()`, or `show()` for sampling
- Write to storage instead of collecting

```python
# Good: Sample data
df.take(100)
df.show(20)

# Avoid: Collect entire dataset
# df.collect()  # Only for small results
```

### 3. Use Lazy Evaluation
- Spark operations are lazy by default
- Chain transformations efficiently
- Action triggers execution

### 4. Filter Early and Often
- Apply filters as early as possible
- Reduces data volume in subsequent operations
- Combine multiple filters

```python
# Good: Filter early
df.filter(col("date") >= "2024-01-01") \
  .filter(col("status") == "active") \
  .groupBy("region").count()

# Better: Combine filters
df.filter((col("date") >= "2024-01-01") & (col("status") == "active")) \
  .groupBy("region").count()
```

### 5. Avoid Nested Loops
- Use joins and aggregations instead
- Leverage Spark's distributed processing
- Vectorize operations when possible

---

## Data Serialization

### 1. Use Kryo Serialization
- Faster than Java serialization
- Reduces serialization overhead
- Register custom classes

```python
spark.conf.set("spark.serializer", "org.apache.spark.serializer.KryoSerializer")
spark.conf.set("spark.kryo.registrationRequired", "false")
```

### 2. Optimize Data Types
- Use appropriate data types (avoid strings for IDs)
- Prefer primitive types over complex objects
- Use arrays/maps efficiently

---

## File Format Optimizations

### 1. Use Columnar Formats
- Prefer Parquet over CSV/JSON
- Columnar storage enables predicate pushdown
- Better compression and performance

```python
# Good: Parquet format
df.write.parquet("output_path")

# Better: Parquet with compression
df.write.option("compression", "snappy").parquet("output_path")
```

### 2. Optimize File Sizes
- Target file size: 128MB - 1GB
- Use `coalesce()` or `repartition()` before writing
- Avoid too many small files

```python
# Write with optimal partition count
df.coalesce(100).write.parquet("output_path")
```

### 3. Use Partition Pruning
- Partition data by frequently filtered columns
- Enables partition pruning in queries
- Reduces I/O significantly

```python
# Partitioned write enables partition pruning
df.write.partitionBy("date", "region").parquet("output_path")

# Query benefits from partition pruning
df = spark.read.parquet("output_path").filter(col("date") == "2024-01-01")
```

### 4. Compression Settings
- Use Snappy for balanced compression/speed
- Use Gzip for better compression (slower)
- Use LZ4 for faster compression (less compression)

---

## Query Optimization

### 1. Enable Adaptive Query Execution (AQE)
- Automatically optimizes query execution
- Adjusts join strategies at runtime
- Coalesces small partitions

```python
spark.conf.set("spark.sql.adaptive.enabled", "true")
spark.conf.set("spark.sql.adaptive.coalescePartitions.enabled", "true")
spark.conf.set("spark.sql.adaptive.skewJoin.enabled", "true")
```

### 2. Use Predicate Pushdown
- Filters are pushed to data source when possible
- Reduces data read from storage
- Works with Parquet, ORC, and other formats

### 3. Column Pruning
- Only read required columns
- Reduces I/O and memory usage
- Automatic in Spark SQL

```python
# Only reads required columns
df.select("id", "name", "date").filter(col("date") >= "2024-01-01")
```

### 4. Cost-Based Optimization (CBO)
- Enable for better join order decisions
- Requires table statistics
- Use `ANALYZE TABLE` to collect stats

```python
spark.conf.set("spark.sql.cbo.enabled", "true")
spark.conf.set("spark.sql.cbo.joinReorder.enabled", "true")

# Collect statistics
spark.sql("ANALYZE TABLE table_name COMPUTE STATISTICS FOR ALL COLUMNS")
```

---

## Memory Management

### 1. Configure Memory Fractions
- Balance execution and storage memory
- Adjust based on caching needs
- Monitor memory usage

```python
spark.conf.set("spark.memory.fraction", "0.8")
spark.conf.set("spark.memory.storageFraction", "0.5")
```

### 2. Handle Out-of-Memory Errors
- Increase executor memory
- Reduce partition size
- Use disk-based operations
- Optimize data structures

### 3. Garbage Collection Tuning
- Use G1GC for large heaps
- Tune GC parameters
- Monitor GC times

```python
spark.conf.set("spark.executor.extraJavaOptions", 
    "-XX:+UseG1GC -XX:InitiatingHeapOccupancyPercent=35")
```

---

## Best Practices Checklist

### Before Writing Code
- [ ] Understand data size and distribution
- [ ] Plan partitioning strategy
- [ ] Identify join patterns
- [ ] Determine caching needs

### While Coding
- [ ] Filter data early
- [ ] Use column expressions over UDFs
- [ ] Broadcast small lookup tables
- [ ] Partition by join keys
- [ ] Avoid unnecessary shuffles
- [ ] Use appropriate file formats

### After Coding
- [ ] Review Spark UI for bottlenecks
- [ ] Check for data skew
- [ ] Verify partition sizes
- [ ] Monitor resource utilization
- [ ] Profile query execution plans

### Performance Monitoring
- [ ] Check task execution times
- [ ] Monitor shuffle read/write
- [ ] Review cache hit rates
- [ ] Analyze query execution plans
- [ ] Track memory usage

---

## Common Anti-Patterns to Avoid

1. **Too Many Small Files**: Causes overhead in metadata and task scheduling
2. **Collecting Large Datasets**: Brings all data to driver, causes OOM
3. **Unnecessary Repartitions**: Adds shuffle overhead without benefit
4. **Cartesian Joins**: Explodes data size exponentially
5. **Not Filtering Early**: Processes unnecessary data
6. **Over-caching**: Wastes memory on data used once
7. **Using UDFs Unnecessarily**: Prevents Catalyst optimization
8. **Ignoring Data Skew**: Causes straggler tasks
9. **Wrong Join Strategy**: Using sort-merge when broadcast would work
10. **Not Using Columnar Formats**: Missing compression and predicate pushdown

---

## Quick Reference

### Configuration Settings
```python
# Core optimizations
spark.conf.set("spark.sql.adaptive.enabled", "true")
spark.conf.set("spark.sql.adaptive.coalescePartitions.enabled", "true")
spark.conf.set("spark.sql.adaptive.skewJoin.enabled", "true")
spark.conf.set("spark.sql.adaptive.skewJoin.skewedPartitionThresholdInBytes", "256MB")

# Shuffle optimization
spark.conf.set("spark.sql.shuffle.partitions", "400")
spark.conf.set("spark.sql.shuffle.partitions.enabled", "true")

# Serialization
spark.conf.set("spark.serializer", "org.apache.spark.serializer.KryoSerializer")

# Memory
spark.conf.set("spark.memory.fraction", "0.8")
spark.conf.set("spark.memory.storageFraction", "0.5")

# Dynamic allocation
spark.conf.set("spark.dynamicAllocation.enabled", "true")
spark.conf.set("spark.dynamicAllocation.minExecutors", "2")
spark.conf.set("spark.dynamicAllocation.maxExecutors", "20")
```

### Code Patterns
```python
# Optimal join pattern
df1.repartition("key").join(
    broadcast(df2.repartition("key")), 
    "key"
)

# Optimal aggregation pattern
df.filter(conditions) \
  .repartition("group_key") \
  .groupBy("group_key") \
  .agg(...)

# Optimal write pattern
df.coalesce(100) \
  .write \
  .partitionBy("date") \
  .option("compression", "snappy") \
  .parquet("output_path")
```

---

## Additional Resources

- [Spark SQL Performance Tuning](https://spark.apache.org/docs/latest/sql-performance-tuning.html)
- [Spark Configuration Guide](https://spark.apache.org/docs/latest/configuration.html)
- Monitor Spark UI at `http://driver-node:4040`
- Use `df.explain(True)` to analyze query plans

---

**Last Updated**: February 2025

**Note**: These optimizations should be applied based on your specific use case, data characteristics, and cluster configuration. Always profile and measure performance improvements.
