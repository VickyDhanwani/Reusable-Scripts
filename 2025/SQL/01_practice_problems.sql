/*
𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟏: Write a query to find duplicate rows in a table.

𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟐: How would you determine the Average Revenue Per User (ARPU) from transaction data?

𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟑: Write a query to calculate YoY (Year-over-Year) growth for a set of transactions.

𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟒: Write a query to find customers who have used more than 2 credit cards for transactions in a given month.

𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟓: Imagine you had a table of Capital One employee salary data. Write a SQL query to find the top 3 highest-earning employees within each department.

𝐒𝐐𝐋 𝐐𝐮𝐞𝐬𝐭𝐢𝐨𝐧 𝟔: Write a query to fetch the second-highest salary from an employee table.

https://www.linkedin.com/posts/jaswanth49b057228_sql-dataanalyst-interviewpreparation-activity-7383335495830921216-K1mg?utm_source=share&utm_medium=member_desktop&rcm=ACoAACLJA0YB9IASYMLAeLUD3Dpvn8t4VwKOggA


*/

-- Write a query to find duplicate rows in a table.

with duplicate as (
    Select column1, column2, columnN, ROW_NUMBER() OVER (PARTITION BY column1, column2, columnN) as rownum
    from InputTable
)
select column1, column2, columnN
from duplicate
where rownum > 1

-- How would you determine the Average Revenue Per User (ARPU) from transaction data?

SELECT 
    SUM(Amount) / COUNT(DISTINCT CustomerId) AS ARPU
FROM Sales;

-- Write a query to calculate YoY (Year-over-Year) growth for a set of transactions.

WITH yearly_sales AS (
    SELECT 
        DATE_PART('year', OrderDate) AS order_year,
        SUM(Amount) AS total_sales
    FROM Sales
    GROUP BY DATE_PART('year', OrderDate)
)
SELECT 
    curr.order_year,
    curr.total_sales AS current_year_sales,
    prev.total_sales AS previous_year_sales,
    ROUND(((curr.total_sales - prev.total_sales) * 100.0 / prev.total_sales), 2) AS yoy_growth_percentage
FROM yearly_sales curr
LEFT JOIN yearly_sales prev ON curr.order_year = prev.order_year + 1
ORDER BY curr.order_year;


--Write a query to find customers who have used more than 2 credit cards for transactions in a given month.

with monthly_transactions as (
    select CustomerId, DATE_PART('month', CAST(TransactionDate as DATETIME)) as TxnMonth, 
    DATE_PART('year', CAST(TransactionDate as DATETIME)) as TxnYear, CreditCardID
    from Transactions
)
    select CustomerId, TxnMonth, TxnYear, COUNT(DISTINCT CreditCardID) as Card_Count
    from monthly_transactions
    group by CustomerId, TxnMonth, TxnYear
    having COUNT(DISTINCT CreditCardID) > 2

-- 9 / 10

-- Imagine you had a table of Capital One employee salary data. Write a SQL query to find the top 3 highest-earning employees within each department.

with department_salary_top as (
    select EmployeeId, EmployeeName, Salary, Department, ROW_NUMBER() OVER (PARTITION BY Department ORDER BY Salary DESC) as Dept_Rank
    from capital_One_Employee_Salary
)
select EmployeeId, EmployeeName, Salary, Department
from department_salary_top
where Dept_Rank <= 3

-- 9 / 10

-- Write a query to fetch the second-highest salary from an employee table.

with EmployeeSalary as (
    Select Salary, DENSE_RANK() Over (order by Salary DESC) as Salary_Rank from Employee
)
select top 1 COALESCE(Salary, NULL) AS SecondHighestSalary
FROM EmployeeSalary
WHERE Salary_Rank = 2

-- 8 / 10

