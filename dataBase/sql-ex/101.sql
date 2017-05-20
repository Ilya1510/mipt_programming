WITH PrinterWithGroup AS 
(select *, case when color = 'n' then 1 else 0 end + 
(select count(color) FROM printer 
WHERE code < p.code and color = 'n') AS numGroup
FROM Printer as p)

SELECT code, model, color, type, price, 
(SELECT max(model) FROM PrinterWithGroup 
WHERE numGroup=P0.numGroup) AS max_model, 
(SELECT count(distinct type) FROM PrinterWithGroup 
WHERE numGroup=P0.numGroup) AS distct_types, 
(SELECT avg(price) FROM PrinterWithGroup 
WHERE numGroup=P0.numGroup) AS avg_price
FROM PrinterWithGroup as P0

