/*

Question: Find all employees who earn more than their manager.

*/

SELECT e.EmployeeId, e.Name, ROUND(e.Salary, 2), ROUND(m.Salary, 2)
from Employees as e inner join Employee as m on e.ManagerId = m.EmployeeId and e.Salary > m.Salary
 
-- FASTER Query LAG function

SELECT e.EmployeeId,
        e.Name,
        e.Salary,
        LAG(e.Salary) OVER (PARTITION BY e.ManagerId ORDER BY e.Salary) as ManagerSalary
FRom Employees as e
where e.Salary > LAG(e.Salary) over (PARTITION BY e.ManagerId ORDER BY e.Salary)

