from pyspark.sql import SparkSession

spark = SparkSession.builder.appName("UpsertMerge") \
        .config("spark.sql.extensions", "io.delta.sql.DeltaSparkSessionExtension") \
        .config("spark.sql.catalog.spark_catalog", "org.apache.spark.sql.delta.DeltaCatalog") \
        .getOrCreate()

data1 = [
    ("Amit", "IN", 25),
    ("Neha", "UK", 40),
    ("Raj", "FR", 33)
]

schema = ["name", "country", "age"]

df1 = spark.createDataFrame(data1, schema)
df1.write.format("delta").mode("overwrite").save("delta_people")

spark.read.format("delta").load("delta_people").show()

data2 = [
    ("Amit", "IN", 26), # Update Age
    ("Neha", "UK", 40), # Same
    ("Karan", "US", 44) # New Record
]

df2 = spark.createDataFrame(data2, schema)

deltaTable = DeltaTable.forPath(spark, "delta_people")

deltaTable.alias("target").merge(
    df2.alias("source"),
    "target.name = source.name"
).whenMatchedUpdateAll() \
.whenNotMatchedInsertAll() \
.execute()

spark.read.format("delta").load("delta_people").show()

# Output ->
# [
#     ("Amit", "IN", 26),
#     ("Neha", "UK", 40),
#     ("Raj", "FR", 33),
#     ("Karan", "US", 44)
# ]