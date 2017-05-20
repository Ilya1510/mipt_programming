SELECT My.class
FROM (select Classes.*, Ships.name
FROM Classes INNER JOIN Ships on (Classes.class = Ships.class)
UNION
select Classes.*, Outcomes.ship
from Classes INNER JOIN Outcomes on (Classes.class = Outcomes.ship)) as My
GROUP BY class
HAVING count(*) = 1

