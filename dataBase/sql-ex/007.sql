SELECT model1, price1 FROM 
Product INNER JOIN
(SELECT model, price FROM PC UNION
SELECT model, price FROM Laptop UNION
SELECT model, price FROM Printer) AS M(model1, price1) ON
model=model1 AND maker='B'
