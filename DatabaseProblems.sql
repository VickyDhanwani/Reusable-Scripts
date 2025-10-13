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