WITH MyG AS (SELECT * FROM Ships)
SELECT name 
from battles 
WHERE year(date) not in 
(select launched 
from ships 
WHERE launched is not NULL)

