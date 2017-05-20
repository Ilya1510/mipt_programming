Select DISTINCT maker, AVG(hd) FROM 
Product INNER JOIN PC ON
Product.model = PC.model AND 
maker IN (SELECT maker FROM
Product WHERE type = 'Printer')
Group by maker

