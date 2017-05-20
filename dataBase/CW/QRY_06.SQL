use AdventureWorks2000

SELECT DISTINCT CountryRegion.Name, Contact.FirstName, COUNT(*) AS cnt FROM 
Contact INNER JOIN storeContact ON
Contact.ContactID=storeContact.ContactID INNER JOIN Store ON 
storeContact.StoreID=Store.StoreID INNER JOIN Customer ON 
Store.CustomerID=Customer.CustomerID INNER JOIN CustomerAddress ON 
Customer.CustomerID=CustomerAddress.CustomerID INNER JOIN Address ON
CustomerAddress.AddressID=Address.AddressID INNER JOIN CountryRegion ON 
Address.CountryRegionCode=CountryRegion.CountryRegionCode
Group BY CountryRegion.Name, Contact.FirstName
ORDER BY cnt desc 