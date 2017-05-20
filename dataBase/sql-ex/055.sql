WITH MyG AS (SELECT * FROM Ships)

select classes.class, min(launched) 
    from classes LEFT JOIN ships 
    on classes.class = ships.class 
       GROUP BY classes.class


