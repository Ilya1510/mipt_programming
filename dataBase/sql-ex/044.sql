WITH MyG AS (SELECT * FROM Ships)
SELECT name 
from ships 
where name like 'R%'
union
select ship 
from outcomes 
where ship like 'R%' and ship not in 
(SELECT name FROM ships)

