WITH MyG AS (SELECT * FROM Ships)

select distinct classes.class 
FROM classes JOIN ships on classes.class = ships.class JOIN outcomes 
        on name = ship where result = 'sunk'
union
select distinct class FROM classes JOIN outcomes 
        on class = ship where result = 'sunk'

