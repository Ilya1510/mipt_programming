WITH MyG AS (SELECT * FROM Ships),
t as (SELECT * 
from (SELECT 1 a1 union all 
SELECT 2 union all 
SELECT 3 union all 
SELECT 4 union all 
SELECT 5 union all 
SELECT 6 union all 
SELECT 7 union all 
SELECT 8 union all 
SELECT 9 union all 
SELECT 10) as x1,
(SELECT 1 a2 
union all 
SELECT 2 union all 
SELECT 3 union all 
SELECT 4 union all 
SELECT 5 union all 
SELECT 6 union all 
SELECT 7 union all 
SELECT 8 union all 
SELECT 9 union all 
SELECT 10) as x2,
(SELECT 1 a3 union all SELECT 2 union all SELECT 3 union all SELECT 4 union all SELECT 5 union all SELECT 6 union all SELECT 7 union all SELECT 8 union all SELECT 9 union all SELECT 10) as x3)

SELECT class, replace('bc-x', 'x', cast(100*(a1-1) + 10*(a2-1) + a3 as char(4))) as num
from t cross join classes
where type = 'bc' and (100*(a1-1) + 10*(a2-1) + a3 <= numguns)

