SELECT DISTINCT maker, speed FROM
Product INNER JOIN Laptop
ON Product.type='Laptop' AND Product.model=Laptop.model AND hd>=10
