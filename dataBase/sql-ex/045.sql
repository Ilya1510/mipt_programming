WITH MyG AS (SELECT * FROM Ships)
SELECT name from ships 
where name like '% % %'
union
select ship from outcomes 
where ship like '% % %' and ship not in 
(select name FROM ships)

