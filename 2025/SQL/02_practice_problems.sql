/*
1. You need to retrieve the names and salaries of employees from the employees
table, but only for those working in the Finance department.
2. You need to increase the salary of all employees in the IT department by 10%.
3. Write a SQL query to find all duplicate emails in a table named Person.
4. Write a SQL query to get the second highest salary from the Employee table.
5. Write an SQL query to find all numbers that appear at least three times consecutively.
6. Given the Employee table, write a SQL query that finds out employees who earn more than their managers.
*/

SELECT e.name as Name, ROUND(e.Salary, 2) as Salary
from Employee as e inner join Department as d on e.department_id = d.id
where d.name = 'Finance'

UPDATE Employee
SET Salary = 1.1 * Salary
WHERE department_id in (Select id from Department where name = 'IT')

Select emails
from Person
group by emails
having count(*) > 1

with Salary_Rank as (
    Select Salary, DENSE_RANK() OVER (Order by Salary DESC) as Rnk
    From Employee
)
select Max(Salary) as SecondHighestSalary
from Salary_Rank
where Rnk = 2

with rownum as (
    select *, ROW_NUMBER() OVER (order by id) as rn
    From Numbers
)
select DISTINCT a.number
from rownum as a join rownum as b on a.number = b.number and a.rn = b.rn - 1 
join rownum as c on b.number = c.number and  b.rn = c.rn - 1


-- Best approach: Using LEAD (FAANG standard)
WITH consecutive_check AS (
    SELECT 
        num,
        LEAD(num, 1) OVER (ORDER BY id) AS next_num,
        LEAD(num, 2) OVER (ORDER BY id) AS next_next_num
    FROM Logs
)
SELECT DISTINCT num AS ConsecutiveNums
FROM consecutive_check
WHERE num = next_num AND num = next_next_num;

Select e.EmployeeName, e.EmployeeId
from Employee as e inner join Employee as m on e.manager_id = m.id
and e.Salary > m.Salary