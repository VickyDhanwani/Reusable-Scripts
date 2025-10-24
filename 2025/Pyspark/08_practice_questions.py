"""
You have a PySpark DataFrame with columns: employee_id, name, department, and salary.
Write PySpark code to:

Filter employees whose salary is greater than 50000
Select only the name and salary columns
"""

from pyspark.sql import SparkSession
from pyspark.sql.types import IntegerType, StringType, FloatType, StructType, StructField
from pyspark.sql.functions import col
spark = SparkSession.builder.appName("problemset").getOrCreate()

schema = StructType([
    StructField("employee_id", IntegerType()),
    StructField("name", StringType()),
    StructField("department", StringType()),
    StructField("salary", FloatType())
])

data = [
    (1, "john", "IT", 120000.00),
    (2, "ale", "HR", 47000.00),
    (3, "brad", "Admin", 140000.00)
]

df = spark.createDataFrame(data, schema)

df_filtered = df.filter(col("salary") > 50000.00)
df_selected = df_filtered.select("name", "salary")
df_selected.show()

"""
You have a DataFrame with columns: order_id, customer_id, product, quantity, price
Write PySpark code to:

Add a new column called total_amount (quantity * price)
Group by customer_id and calculate the total spending per customer
Sort the results by total spending in descending order
"""

from pyspark.sql import SparkSession
from pyspark.sql.types import StructField, StructType, StringType, FloatType
from pyspark.sql.functions import col, round, sum as agg_sum

spark = SparkSession.builder.appName("practiceDataset").getOrCreate()
schema = StructType([
    StructField("order_id", StringType()),
    StructField("customer_id", StringType()),
    StructField("product", StringType()),
    StructField("quantity", FloatType()),
    StructField("price", FloatType())
])

df = spark.read.format("csv").schema(schema).option("header", "True").load("dbfs:/raw/data/orders.csv")
df = df.withColumn("total_amount", round(col("quantity") * col("price"), 2)).groupBy("customer_id").agg(agg_sum("total_amount").alias("total_spending")).orderBy(col("total_spending").desc())
df.show()