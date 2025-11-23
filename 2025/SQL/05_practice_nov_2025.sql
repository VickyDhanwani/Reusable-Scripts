/*Write a SQL query to find the top 5 employees by total sales in the last 30 days, 
and include their department names. Tables:

employees (id, name, department_id)

departments (id, name)

sales (id, employee_id, sale_date, amount)
*/

Select e.name, e.id, d.name,  sum(s.amount)
FROM sales as s join employees as e on (s.employee_id = e.id 
and s.sale_date >= CURRENT_DATE - INTERVAL '30 days')
inner join departments as d on e.department_id = d.id
Group by e.name, e.id, d.name
order by 4 desc
limit 5