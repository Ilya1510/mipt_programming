WITH MyG AS (SELECT * FROM Ships)

SELECT Mak.maker, T.type, 
CAST(CAST((SELECT COUNT(*) 
FROM Product 
WHERE (maker = Mak.maker) 
AND type = T.type) * 100 AS NUMERIC(6,2)) / 
(SELECT COUNT(*) 
FROM Product 
WHERE (maker = Mak.maker)) AS NUMERIC(6,2)) AS prc
FROM (SELECT DISTINCT maker 
FROM Product) AS Mak, 
(SELECT DISTINCT type 
FROM Product) AS T

