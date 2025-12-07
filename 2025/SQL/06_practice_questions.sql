--Calculate the day-over-day retention rate of users.
WITH cte AS (
    SELECT 
        FORMAT([Date], 'dd-MM-yyyy') AS [Date], 
        COUNT([Users]) AS UserCount
    FROM Users
    GROUP BY [Date]
)
SELECT 
    [Date], 
    ROUND(
        (1.0 * [UserCount]) / NULLIF(LAG([UserCount]) OVER (ORDER BY [Date]), 0), 
        2
    ) AS RateOfRetention
FROM cte

--Find the top 3 content creators by engagement in the last 7 days.

WITH Last7Days AS (
    SELECT CreatorID, CreatorName, [DateTime], Views
    FROM ViewCount
    WHERE [DateTime] >= DATEADD(DAY, -6, CAST(GETDATE() AS DATE)) -- last 7 days including today
)
SELECT TOP 3
    CreatorID,
    CreatorName,
    SUM(Views) AS TotalEngagement
FROM Last7Days
GROUP BY CreatorID, CreatorName
ORDER BY TotalEngagement DESC;

--Write a query to identify "resurrected" users (users who were inactive for 30 days and came back).

with CTE as (
    SELECT UserId, LoginDate, LAG(LoginDate) OVER (PARTITION BY UserId ORDER BY LoginDate) as PreviousLoginDate
    FROM UserActivity
)
SELECT UserId
from CTE
Where DATEDIFF(DAY, PreviousLoginDate, LoginDate) > 30

--Write a query to calculate the rolling 3-month average revenue per user.

