"""
Explain data skewness in Spark. How does it impact job performance? How would you detect and fix it?

My answer - 

a. Data Skewness refers to uneven distribution of data across the partitions in worker nodes in spark cluster.
b. One single partition holds high volume of data whereas other partitions hold comparatively very low volumns.
c. Skewness impacts query performance as single parition takes large time and resources and parallelism is defeated.
d. One single partition having skew will impact the job in following way 
- Partition takes large time than others for aggregation
- Join operation if executed will consume high resources and cause shuffling of data if join key is not used correctly.
- Out of Memory issue might occur if single parittion exceeds the allowable memory of worker node.
e. To detect the skew, we need to check the count of records per partition 
f. Count the records by the key they were distributed in ppartitions to identify if any worker has high volume.
g. Spark UI can be used to identify which job is taking significantly longer.
h. to fix the skew, we can repartition the data evenly across partition, however this is a costly operation as it requires shuffle.
i. Enable AQE to auto detect skewness while execution. 
j. Use salting to create key in data frame and resdistribute data across salt keys to avoid skewness.

Actual answer - 

"Data skewness occurs when data is unevenly distributed across partitions. For example, if 90% of events come from a single country, one partition gets overloaded while others are idle. This defeats parallelism and causes:
Detection: I check Spark UI for task durations—if one task takes 10x longer, that's skew. Or I count records per partition using RDD/SQL.
Fixes (in order of preference):

AQE - Enable spark.sql.adaptive.skewJoin.enabled, automatic and requires no code change
Salting - Add random suffix to skewed keys (e.g., NULL values get salt_0 to salt_9)
Broadcast Join - If small table available
Repartition - Last resort, costly shuffle operation"

"""

# Detection

# Method 1: Check records per partition
df.rdd.mapPartitions(lambda x: [sum(1 for _ in x)]).collect()
# Output: [1000000, 50000, 45000, 48000] ← SKEW DETECTED!

# Method 2: Using SQL
spark.sql("""
SELECT partition_key, COUNT(*) as cnt
FROM your_table
GROUP BY partition_key
ORDER BY cnt DESC
""")

# Method 3: Spark UI
# - Look at Task stage metrics
# - If 1 task takes 10x longer than others → skew

# Problem: Heavy skew on NULL values
df_skewed = df.where("user_id IS NULL")  # 90% of data

# Solution: Add random salt to NULL keys
from pyspark.sql.functions import rand, concat, lit

df_salted = df.withColumn(
    "salted_key",
    when(col("user_id").isNull(), concat(lit("salt_"), (rand() * 10).cast("int")))
    .otherwise(col("user_id"))
)

# Now NULL data is distributed across 10 partitions instead of 1
df_result = df_salted.repartition(200, "salted_key")

from pyspark.sql.functions import broadcast

# If one side is small, broadcast it instead of shuffling both
result = df_large.join(broadcast(df_small), "key")
# Avoids shuffle entirely, no skew risk

# Enable in Spark 3.0+
spark.conf.set("spark.sql.adaptive.enabled", "true")
spark.conf.set("spark.sql.adaptive.skewJoin.enabled", "true")
spark.conf.set("spark.sql.adaptive.skewJoin.skewFactor", 5.0)  # if partition is 5x larger

# AQE automatically detects and splits skewed partitions during execution

# Instead of random salting, use domain knowledge
df_salted = df.withColumn(
    "custom_partition_key",
    when(col("country") == "US", concat(lit("US_"), (hash(col("user_id")) % 50)))
    .when(col("country") == "IN", concat(lit("IN_"), (hash(col("user_id")) % 20)))
    .otherwise(col("country"))
)