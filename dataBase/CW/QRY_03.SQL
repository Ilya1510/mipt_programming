use AdventureWorks2000

SELECT COUNT(*)
FROM (Select Customer.CustomerID 
FROM Customer INNER JOIN CustomerAddress ON 
Customer.CustomerID=CustomerAddress.CustomerID 
GROUP BY Customer.CustomerID 
Having Count(*) > 1) AS addresGroupCustomer