use AdventureWorks2000

SELECT DISTINCT Customer.CustomerID 
FROM Customer INNER JOIN SalesOrderHeader as Sales ON 
Customer.CustomerID=Sales.CustomerID 
WHERE Customer.CustomerID not IN (
SELECT DISTINCT Customer.CustomerID 
FROM Customer INNER JOIN SalesOrderHeader as Sales ON 
Customer.CustomerID=Sales.CustomerID WHERE
OrderDate between '2004-01-01' and getdate())