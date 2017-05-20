use AdventureWorks2000

SELECT DISTINCT Contact.FirstName, Address.city FROM 
Contact INNER JOIN storeContact ON
Contact.ContactID=storeContact.ContactID INNER JOIN Store ON 
storeContact.StoreID=Store.StoreID INNER JOIN Customer ON 
Store.CustomerID=Customer.CustomerID INNER JOIN CustomerAddress ON 
Customer.CustomerID=CustomerAddress.CustomerID INNER JOIN Address ON
CustomerAddress.AddressID=Address.AddressID WHERE 
CustomerType='S' 
ORDER BY FirstName, Address.city