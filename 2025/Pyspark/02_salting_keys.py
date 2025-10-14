from pyspark.sql import SparkSession
from pyspark.sql.functions import col, rand

spark = SparkSession.builder.appName("saltDF") \
        .config("spark.sql.adaptive.enabled", "true") \
        .config("spark.sql.adaptive.coleasePartitions.enabled", "true").getOrCreate()

df = spark.read.option("headers","true").option("inferSchema", "true").csv("/path/input/file.cs")
df_with_salting = df.withColumn("salted_key", (col("id") + (rand() * 10).cast("int")))