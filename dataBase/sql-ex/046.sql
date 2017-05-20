with BattleShips as (
SELECT distinct ship, displacement, numGuns 
from classes join ships on classes.class = ships.class 
JOIN outcomes on name = ship WHERE battle = 'Guadalcanal'
UNION
SELECT distinct ship, displacement, numGuns from classes join outcomes on classes.class = outcomes.ship where battle = 'Guadalcanal'
)

SELECT * from BattleShips
UNION
SELECT ship, NULL as displacement, NULL as numGuns from outcomes 
where battle = 'Guadalcanal' and ship not in 
                                 (SELECT ship from BattleShips)

