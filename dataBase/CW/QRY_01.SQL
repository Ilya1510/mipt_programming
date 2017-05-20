use AdventureWorks2000

Select CustomerType, Count(*) as cnt
FROM Customer 
Group by CustomerType 