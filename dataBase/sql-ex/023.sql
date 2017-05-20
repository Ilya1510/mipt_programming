SELECT maker FROM (Product INNER JOIN PC ON
Product.model=PC.model AND speed>=750) INTERSECT
SELECT maker FROM (Product INNER JOIN Laptop ON
Product.model=Laptop.model AND speed>=750)

