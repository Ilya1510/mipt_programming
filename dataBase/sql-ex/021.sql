SELECT maker, MAX(price) FROM
Product INNER JOIN PC ON
Product.model=PC.model AND Product.type='PC'
GROUP BY maker

