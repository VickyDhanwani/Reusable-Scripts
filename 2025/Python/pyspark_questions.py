"""
1. Write an SQL query to find employees who earn more than their direct manager.
2. From a transactions table, write a query to identify users who placed more than 20 critical orders every week for at least 1 month.
3. Given a nested JSON file, how would you parse and flatten it in PySpark?
4. How would you design a cost-efficient pipeline in Azure using adf, databricks, and adls?
5. Explain dimensional modeling and how you would design a sales fact table with product and customer dimensions.
"""

# 1. SQL query to find employees who earn more than their direct manager

from pyspark.sql import SparkSession
from pyspark.sql.functions import col

spark = SparkSession.builder.appName("main-app").getOrCreate()

# Sample employee data
data = [
    (1, "Alice", 5000, None),  # CEO
    (2, "Bob", 4000, 1),       # Manager under Alice
    (3, "Charlie", 4500, 2),   # Employee under Bob
    (4, "David", 3000, 2),     # Employee under Bob
    (5, "Eve", 6000, 1)        # Employee under Alice
]
columns = ["emp_id", "name", "salary", "manager_id"]
df = spark.createDataFrame(data, columns)
df.createOrReplaceTempView("employees")
query = """
SELECT e1.emp_id, e1.name, e1.salary, e2.name AS manager
FROM employees e1
JOIN employees e2 ON e1.manager_id = e2.emp_id
WHERE e1.salary > e2.salary
"""
result = spark.sql(query)
result.show()
# 2. SQL query to identify users who placed more than 20 critical orders every week for at least 1 month    
# Sample transactions data
data = [
    (1, "2023-01-01", "critical"),
    (1, "2023-01-02", "normal"),
    (1, "2023-01-03", "critical"),
    (1, "2023-01-04", "critical"),
    (1, "2023-01-05", "critical"),
    (1, "2023-01-06", "critical"),
    (1, "2023-01-07", "critical"),
    (1, "2023-01-08", "critical"),
    (1, "2023-01-09", "critical"),
    (1, "2023-01-10", "critical"),
    (1, "2023-01-11", "critical"),
    (1, "2023-01-12", "critical"),
    (1, "2023-01-13", "critical"),
    (1, "2023-01-14", "critical"),
    (1, "2023-01-15", "critical"),
    (1, "2023-01-16", "critical"),
    (1, "2023-01-17", "critical"),
    (1, "2023-01-18", "critical"),
    (1, "2023-01-19", "critical"),
    (1, "2023-01-20", "critical"),
    (1, "2023-01-21", "critical"),  # 21 critical orders in first week
    (2, "2023-01-01", "normal"),
    (2, "2023-01-02", "normal"),
    (2, "2023-01-03", "normal")
]
columns = ["user_id", "order_date", "order_type"]
df = spark.createDataFrame(data, columns)
df.createOrReplaceTempView("transactions")
query = """
SELECT user_id
FROM (
    SELECT user_id, WEEK(order_date) AS order_week, COUNT(*) AS critical_orders
    FROM transactions
    WHERE order_type = 'critical'
    GROUP BY user_id, WEEK(order_date)
    HAVING COUNT(*) > 20
) AS weekly_critical_orders
GROUP BY user_id
"""
result = spark.sql(query)

result.show()
# 3. Parsing and flattening a nested JSON file in PySpark

df = spark.read.json("path/to/nested.json")
df.printSchema()
from pyspark.sql.functions import explode, col

# Example of flattening a nested structure
flattened_df = df.select(
    col("id"),
    col("name"),
    explode(col("items")).alias("item")
).select(
    col("id"),
    col("name"),
    col("item.item_id").alias("item_id"),
    col("item.item_name").alias("item_name")
)
flattened_df.show()
# 4. Designing a cost-efficient pipeline in Azure using ADF, Databricks, and ADLS
"""
- Use Azure Data Factory (ADF) for orchestrating data workflows and scheduling.
- Store raw and processed data in Azure Data Lake Storage (ADLS) for scalable and cost-effective storage.
- Use Azure Databricks for data processing and transformation tasks, leveraging its optimized Spark environment.
- Implement data partitioning and compression in ADLS to reduce storage costs.
- Monitor and optimize pipeline performance using Azure Monitor and ADF built-in monitoring tools.
"""
# 5. Dimensional modeling and designing a sales fact table with product and customer dimensions
"""
- Dimensional modeling involves structuring data into fact and dimension tables to optimize query performance and ease of analysis.
- A sales fact table would contain measurable, quantitative data about sales transactions, such as sales_amount, quantity_sold, and transaction_date.
- The product dimension would include attributes like product_id, product_name, category, and price.
- The customer dimension would include attributes like customer_id, customer_name, location, and segment.
- The fact table would have foreign keys referencing the product and customer dimensions, enabling efficient joins and aggregations for reporting and analysis.
"""
class MyClass:
    def __init__(self, num):
        self.num = num
    
    def user_functions(self):
        self.num = 100
        return f"{self.num}"
obj = MyClass(10)
obj.user_functions()
print(obj.num)
obj = MyClass(10)
obj.user_functions()
print(obj.num)
