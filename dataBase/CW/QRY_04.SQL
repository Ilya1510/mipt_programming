use AdventureWorks2000

SELECT FirstName, MiddleName, LastName, Phone
FROM (individual INNER JOIN Customer ON 
Individual.CustomerID=Customer.CustomerID INNER JOIN customerAddress ON 
customerAddress.AddressID=Customer.CustomerID INNER JOIN Address ON 
CustomerAddress.AddressID=Address.AddressID) WHERE 
CustomerType='I' AND Phone LIKE '1 (11) 500 555-01%' 
Order By Phone desc