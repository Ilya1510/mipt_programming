SELECT AVG(price) FROM 
Product INNER JOIN (
Select model, price FROM PC UNION ALL
SELECT model, price from Laptop) AS M ON
Product.model = M.model AND Product.maker = 'A' AND (type = 'PC' OR type = 'Laptop')

