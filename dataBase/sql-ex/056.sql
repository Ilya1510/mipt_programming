WITH MyG AS (SELECT * FROM Ships),


piup as (
	SELECT classes.class as cls, ships.name from classes 
join ships on classes.class = ships.class 
join outcomes on ships.name = outcomes.ship 
join battles on battles.name = outcomes.battle 
where result = 'sunk'
	union
	SELECT distinct class as cls, ship 
from classes join outcomes on classes.class = outcomes.ship 
where result = 'sunk'
),
    
countSunkShips as (
SELECT cls, count(cls) as cnt from piup group by cls
)
SELECT class,
 case when cnt is NULL then 0 else cnt end cnt
from classes left join countSunkShips on class = cls

