WITH MyG AS (SELECT * FROM Ships)
select DISTINCT battle 
from(
select battle, ship, country 
from outcomes, ships, classes
where (ships.class = classes.class) 
and (outcomes.ship = ships.name) 

union

select battle, ship, country 
from outcomes, classes
where ship in
(select class 
from classes) 
and (classes.class = ship)
) as XXX1
group by battle, country
having count(ship) >= 3

