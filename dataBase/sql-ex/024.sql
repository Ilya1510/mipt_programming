SELECT model FROM 
(SELECT model, price FROM PC UNION
SELECT model, price FROM Laptop UNION
SELECT model, price FROM Printer) AS M
WHERE price>=(SELECT MAX(price) FROM
(SELECT model, price FROM PC UNION
SELECT model, price FROM Laptop UNION
SELECT model, price FROM Printer) AS M)

