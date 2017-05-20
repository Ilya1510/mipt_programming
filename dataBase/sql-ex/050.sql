WITH MyG AS (SELECT * FROM Ships)

select distinct battles.name from classes 
    JOIN ships on classes.class = ships.class and 
                  classes.class = 'Kongo' JOIN outcomes 
        on outcomes.ship = ships.name JOIN battles 
        on battles.name = outcomes.battle

