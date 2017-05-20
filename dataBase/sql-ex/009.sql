SELECT DISTINCT maker FROM
Product INNER JOIN PC ON
Product.model=PC.model AND Product.type='PC' AND speed>=450

