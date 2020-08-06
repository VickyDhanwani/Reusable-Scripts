/*The Employee table holds all employees. Every employee has an Id, a salary, and there is also a column for the department Id.*/
/* The Department table holds all departments of the company.*/
/* Write a SQL query to find employees who have the highest salary in each of the departments.*/

select C.Name as 'Department', A.Name as 'Employee', A.Salary from Employee as A, Department as C
where A.Salary >= all(select B.Salary from Employee as B where B.DepartmentId = A.DepartmentId ) and A.DepartmentId = C.Id;


/*Suppose that a website contains two tables, the Customers table and the Orders table. Write a SQL query to find all customers who never order anything.*/

select Name as Customers from Customers as C
where C.Id not in (select O.CustomerId from Orders as O);
