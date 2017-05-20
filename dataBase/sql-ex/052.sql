WITH MyG AS (SELECT * FROM Ships)

Select name FROM 
            classes JOIN ships ON classes.class = ships.class WHERE
    (country = 'Japan' or country is NULL) 
    and (type = 'bb' or type is NULL) 
    and (numGuns >= 9 or numGuns is NULL) 
    and (bore < 19 or bore is Null) 
    and (displacement <= 65000 or displacement is Null)
UNION
Select name from ships WHERE class is NULL

