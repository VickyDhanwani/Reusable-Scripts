# Should I ask about coalesce() vs repartition()

"""
My answer

- Coalesce combines multiple partitions into one without shuffling.
- Repartition evenly distributes a single paritition into multiple based on number of partitions and / or key provided.
- df.coalesce(10) -> used to combine all partitions of df to 10
- df.repartition(100, "key") -> used to redistribute the df in 100 partitions evenly using "key" column
- Repartition is costly than coalesce as it needs shuffling
- coalesce can be used to optimize the small file problem and reach 128 MB partition size for optimmized processing.
- Repartition can be used to remove data skewness by salting and partitioning single large partition into multiple.
- coalesce is used for optimization in AQE

Excellent answer! Rating: 9/10 ✅
What you nailed:

Core distinction - coalesce (no shuffle) vs repartition (shuffle) ✅
Use cases - small file problem, skewness fixes ✅
Performance impact - correctly identified repartition as costly ✅
Practical applications - 128 MB partition size, AQE optimization ✅
Clear and concise - easy to understand explanation ✅

"coalesce() merges partitions without shuffling, so it's fast but can only reduce partition count. Use it for output optimization or fixing small file problems.
repartition() redistributes data across partitions using shuffle, so it's slower but more flexible—can increase or decrease partitions. Use it when you need specific data distribution, like before a join or to fix skewness.
Example: If I have 1000 partitions with small files, I'd coalesce(50) for final output. But if I need to join on a key, I'd repartition(200, 'key') to ensure proper distribution for the join.'"

"""

"""
coalesce(n): Can ONLY reduce partitions (n < current partitions)
  - Cannot increase partitions (coalesce(1000) on 100-partition df does nothing)
  
repartition(n): Can increase OR decrease partitions
  - More flexible but always triggers shuffle

Use COALESCE when:
- Reducing partitions for final output
- Fixing small file problem (HDFS block size ~128MB)
- Already have right data distribution, just combine partitions
- Cost matters (no shuffle overhead)

Use REPARTITION when:
- Increasing partitions (coalesce can't do this)
- Preparing for a downstream join on specific key
- Need to fix data skewness by distributing on new key
- Data needs to be sorted/bucketed for optimization

coalesce vs repartition on large data:
- coalesce(1) followed by repartition(1000) 
  = First reduces files, then distributes → 2 shuffles (inefficient!)
- Better: Use repartition(1000) directly → 1 shuffle

"""

# coalesce - use case
df.rdd.getNumPartitions()  # 1000
df_optimized = df.coalesce(50)  # Combine to 50, NO shuffle
# Good for: Final output, reducing small files

# repartition - use case
df_balanced = df.repartition(200, "user_id")  # Shuffle & redistribute
# Good for: Preparing for joins, fixing skewness
