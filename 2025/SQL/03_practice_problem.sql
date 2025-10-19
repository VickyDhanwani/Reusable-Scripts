-- Sql question on find managers who are not in same location as their employees ( self join )

/*

https://leetcode.com/discuss/post/5147609/google-l4-application-engineer-data-hyde-k5u3/

*/

SELECT distinct m.id, m.name, m.location
from employees as e join employees as m on e.manager_id = m.id and e.location != m.location

/*
Given a employee table employeeid and managerid , employee name. Find hierarchy for an employeeid upto highest level. 
- Recursive CTEs concept
*/

WITH RECURSIVE cte AS (
    -- Base case: Start with the given employee
    SELECT employeeid, name, managerid, 1 AS level
    FROM employee
    WHERE employeeid = @employee_id
    
    UNION ALL
    
    -- Recursive case: Get the manager's manager, and so on
    SELECT e.employeeid, e.name, e.managerid, c.level + 1
    FROM employee e
    INNER JOIN cte c ON e.employeeid = c.managerid
    WHERE c.managerid IS NOT NULL
)
SELECT employeeid, name, managerid, level
FROM cte
ORDER BY level;

/*

https://leetcode.com/discuss/post/5147799/microsoft-data-engineer-senior-software-089eb/

*/

/*
Design a schema for hours spent on application using check in and check out timestamps by employee code and few SQL questions on the same.
*/

CREATE TABLE time_spent_on_application (
    checkin_id INT PRIMARY KEY AUTO_INCREMENT,
    employee_code VARCHAR(20) NOT NULL,
    checkin_time TIMESTAMP NOT NULL,
    checkout_time TIMESTAMP,
    location_id VARCHAR(100) NOT NULL,
    country_id VARCHAR(10) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    -- Constraints
    CONSTRAINT fk_employee FOREIGN KEY (employee_code) 
        REFERENCES employee(employee_code),
    CONSTRAINT fk_location FOREIGN KEY (location_id) 
        REFERENCES location(location_id),
    CONSTRAINT fk_country FOREIGN KEY (country_id) 
        REFERENCES country(country_id),
    CONSTRAINT chk_checkout_after_checkin 
        CHECK (checkout_time > checkin_time OR checkout_time IS NULL),
    
    -- Indexes for performance
    INDEX idx_employee_code (employee_code),
    INDEX idx_checkin_time (checkin_time)
);

-- Separate table for audit/session info
CREATE TABLE checkin_audit (
    audit_id INT PRIMARY KEY AUTO_INCREMENT,
    checkin_id INT NOT NULL,
    ip_address VARCHAR(45),
    user_agent VARCHAR(255),
    
    FOREIGN KEY (checkin_id) 
        REFERENCES time_spent_on_application(checkin_id)
);