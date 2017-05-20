WITH MyG AS (SELECT * FROM Ships)

select name from classes JOIN ships on classes.class = ships.class where bore = 16
union
select ship from classes JOIN outcomes on class = ship where bore = 16

