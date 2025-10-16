
"""
MEDIUM DIFFICULTY PYSPARK INTERVIEW QUESTION

Problem: Find the top 3 products by revenue for each region, 
but only include products where the average order value is above the global median AOV.

Dataset:
- orders: order_id, product_id, region, quantity, unit_price, order_date
- products: product_id, product_name

Expected output:
- region, product_name, total_revenue, avg_order_value, rank

Constraints:
- Handle null values
- Optimize for large datasets (hint: think about partitioning)
- Order by region, then by rank
"""
from pyspark.sql import SparkSession, Window
from pyspark.sql.functions import (
    col, sum, avg, percentile_approx, dense_rank, 
    desc, round, broadcast
 )

spark = SparkSession.builder()\
    .appName("windowFunctions")\
    .config("spark.sql.adaptive.enabled", "true")\
    .getOrCreate()

# Sample data
orders_data = [
    (1, 101, "North", 2, 50, "2024-01-01"),
    (2, 102, "North", 1, 100, "2024-01-02"),
    (3, 101, "North", 3, 50, "2024-01-03"),
    (4, 103, "South", 1, 200, "2024-01-01"),
    (5, 102, "South", 2, 100, "2024-01-02"),
    (6, 104, "South", 5, 30, "2024-01-03"),
    (7, 101, "East", 1, 50, "2024-01-01"),
    (8, 105, "East", 2, 75, "2024-01-02"),
    (9, 102, "East", 4, 100, "2024-01-03"),
]

products_data = [
    (101, "Laptop"),
    (102, "Mouse"),
    (103, "Monitor"),
    (104, "Keyboard"),
    (105, "Headphones"),
]

df_orders = spark.createDataFrame(orders_data, 
    ["order_id", "product_id", "region", "quantity", "unit_price", "order_date"])

df_products = spark.createDataFrame(products_data, 
    ["product_id", "product_name"])

# Step 1: Calculate order value
orders_with_value = df_orders.withColumn(
    "order_value", 
    col("quantity") * col("unit_price")
)

# Step 2: Calculate global median AOV
global_median_aov = orders_with_value.agg(
    percentile_approx("order_value", 0.5)
).collect()[0][0]

print(f"Global Median AOV: {global_median_aov}")

# Step 3: Aggregate by product and region BEFORE joining products
df_agg = orders_with_value.groupBy("region", "product_id").agg(
    sum("order_value").alias("total_revenue"),
    avg("order_value").alias("avg_order_value")
).filter(col("avg_order_value") > global_median_aov)

# Step 4: Join with products using broadcast for optimization
df_agg = df_agg.join(broadcast(df_products), "product_id", "inner")

# Step 5: Apply window function for ranking
window_spec = Window.partitionBy("region").orderBy(desc("total_revenue"))
result_df = df_agg.withColumn(
    "rank", 
    dense_rank().over(window_spec)
).filter(col("rank") <= 3).select(
    "region", 
    "product_name", 
    round("total_revenue", 2).alias("total_revenue"),
    round("avg_order_value", 2).alias("avg_order_value"), 
    "rank"
).orderBy("region", "rank")

result_df.show()