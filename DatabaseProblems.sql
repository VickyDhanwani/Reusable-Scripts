/*The Employee table holds all employees. Every employee has an Id, a salary, and there is also a column for the department Id.*/
/* The Department table holds all departments of the company.*/
/* Write a SQL query to find employees who have the highest salary in each of the departments.*/

select C.Name as 'Department', A.Name as 'Employee', A.Salary from Employee as A, Department as C
where A.Salary >= all(select B.Salary from Employee as B where B.DepartmentId = A.DepartmentId ) and A.DepartmentId = C.Id;


/*Suppose that a website contains two tables, the Customers table and the Orders table. Write a SQL query to find all customers who never order anything.*/

select Name as Customers from Customers as C
where C.Id not in (select O.CustomerId from Orders as O);

/*
Write an SQL query to find all dates' id with higher temperature compared to its previous dates (yesterday).
Return the result table in any order.

Problem URL : https://leetcode.com/problems/rising-temperature/
*/

SELECT A.id
from Weather as A
where A.Temperature > (select B.Temperature
                     from Weather as B
                     where DATEDIFF(A.recordDate, B.recordDate) = 1);


/*
Write a SQL query to get the second highest salary from the Employee table.

*/

# Write your MySQL query statement below
select MAX(C.Salary) as SecondHighestSalary
from Employee as C
where C.Salary = (
    Select MAX(A.Salary)
    from Employee as A
    where A.Salary < (select Max(B.Salary) from Employee as B)
);

/*
Write a SQL query to get the nth highest salary from the Employee table.
For example, given the above Employee table, the nth highest salary where n = 2 is 200. 
If there is no nth highest salary, then the query should return null.
*/

CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  DECLARE k INT;
  SET k = N - 1;
  RETURN (
      
      # Write your MySQL query statement below.
      SELECT DISTINCT Salary
      FROM Employee
      ORDER BY SALARY DESC
      LIMIT 1 OFFSET k
  );
END

I will give you sql query question in first prompt and then in next prompt will give you my answer. Evaluate my answer for FAANG level data engineer out of 10.
Highlight improvements.

/*
Top 5 products by total sales per region
*/

with cte as (
  SELECT region, product, SUM(sales) as total_sales,
  ROW_NUMBER() OVER (PARTITION by region order by SUM(sales) DESC) as rnk
  from Sales 
  group by region, product
)
select region, product, total_sales
from cte
where rnk <= 5

/*
Rating 8.5 Perplexity , 8.5 GPT , 8 Claude
Answer Query 
*/

WITH cte AS (
  SELECT 
    region, 
    product, 
    SUM(COALESCE(sales, 0)) as total_sales,
    DENSE_RANK() OVER (
      PARTITION BY region 
      ORDER BY SUM(COALESCE(sales, 0)) DESC
    ) as rnk
  FROM Sales 
  WHERE sales IS NOT NULL  -- Filter bad data early
  GROUP BY region, product
)
SELECT 
  region, 
  product, 
  total_sales,
  rnk  -- Optional: show rank for clarity
FROM cte
WHERE rnk <= 5
ORDER BY region, rnk;  -- Consistent output ordering

/*
Orders where shipping took longer than the monthly average
*/
with monthly_average as (
  Select DATE_PART(YEAR, CAST(OrderDate AS DATETIME)) as OrderYear, 
    DATE_PART(MONTH, CAST(OrderDate as DATETIME)) as OrderMonth,
    AVG(DATEDIFF(DAY, CAST(OrderDate as DATETIME), CAST(DeliveryDate as DATETIME))) as AverageDeliveryTime
    from Orders
    GROUP BY DATE_PART(YEAR,  CAST(OrderDate as DATETIME)), DATE_PART(MONTH, CAST(OrderDate as DATETIME))
)
Select o.OrderId, o.OrderName, o.OrderDetails
from Orders as o
where Exists (
  Select 1
  from monthly_average as mavg
  where mavg.OrderYear = DATE_PART(YEAR, CAST(o.OrderDate as DATETIME)) 
  and mavg.OrderMonth = DATE_PART(MONTH, CAST(o.OrderDate as DATETIME))
  and DATEDIFF(DAY, CAST(o.OrderDate as DATETIME), CAST(o.DeliveryDate as DATETIME)) > mavg.AverageDeliveryTime
)


/*
Rating - 7.5 Perplexity , 8.8 GPT , 6.5 Claude
Answer Query
*/

WITH cleaned_orders AS (
  SELECT 
    OrderId,
    OrderName,
    OrderDetails,
    CAST(OrderDate AS DATETIME) AS order_dt,
    CAST(DeliveryDate AS DATETIME) AS delivery_dt,
    DATE_PART('YEAR', CAST(OrderDate AS DATETIME)) AS order_year,
    DATE_PART('MONTH', CAST(OrderDate AS DATETIME)) AS order_month
  FROM Orders
  WHERE DeliveryDate IS NOT NULL
    AND DeliveryDate >= OrderDate  -- Data quality check
),
shipping_duration AS (
  SELECT 
    *,
    DATEDIFF('DAY', order_dt, delivery_dt) AS shipping_days
  FROM cleaned_orders
),
monthly_avg AS (
  SELECT 
    order_year,
    order_month,
    AVG(shipping_days) AS avg_shipping_days
  FROM shipping_duration
  GROUP BY order_year, order_month
)
SELECT 
  s.OrderId,
  s.OrderName,
  s.order_dt,
  s.delivery_dt,
  s.shipping_days AS actual_shipping_days,
  m.avg_shipping_days AS monthly_avg_days,
  ROUND(s.shipping_days - m.avg_shipping_days, 2) AS days_over_avg
FROM shipping_duration s
INNER JOIN monthly_avg m
  ON s.order_year = m.order_year
  AND s.order_month = m.order_month
WHERE s.shipping_days > m.avg_shipping_days
ORDER BY s.order_year, s.order_month, days_over_avg DESC;

Give all answers in single piece of code, no explanation needed, just most optimized answer
/*
Recursive query: list all subordinates for a manager
*/

with Recursive subordinates as (
  select e.name,
  e.id,
  e.manager_id,
  0 as level
  from employees as e
  where e.id = :managers_employee_id

  union all

  select e.name,
  e.id,
  e.manager_id,
  s.level + 1 as level
  from employees as e
  inner join subordinates as s on e.manager_id = s.id
)
select name, id, manager_id, level
from subordinates
order by level, id

/*
Deduplicate a huge table while keeping only the latest record
*/
MERGE INTO employee t
USING (
  SELECT 
    id,
    name,
    email,
    gender,
    load_update_timestamp,
    ROW_NUMBER() OVER (
      PARTITION BY id 
      ORDER BY CAST(load_update_timestamp AS TIMESTAMP) DESC
    ) AS rn
  FROM employee
) s
ON t.id = s.id 
  AND t.load_update_timestamp = s.load_update_timestamp
WHEN MATCHED AND s.rn > 1 THEN DELETE;