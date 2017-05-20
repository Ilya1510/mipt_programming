SELECT DISTINCT maker, price FROM
Product INNER JOIN Printer ON
Product.type='Printer' AND Product.model=Printer.model AND color='y'
WHERE price=(SELECT MIN(price) FROM Printer WHERE color='y')

