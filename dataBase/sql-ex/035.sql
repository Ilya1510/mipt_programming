SELECT model, type
FROM product
WHERE upper(model) NOT like '%[^A-Z]%'
OR model not like '%[^0-9]%'

