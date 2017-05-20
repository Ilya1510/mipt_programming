WITH new AS (SELECT * FROM Ships)

select distinct My1.ship
from Outcomes as My1 INNER JOIN Battles as My2 on (My2.name = My1.battle)
where (My1.result = 'damaged') and exists ( select Outcomes.ship
from Outcomes INNER JOIN Battles on (Battles.name = Outcomes.battle)
where (My1.ship = Outcomes.ship) and (Battles.date > My2.date) )

