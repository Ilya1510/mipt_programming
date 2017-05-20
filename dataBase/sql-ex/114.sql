WITH MyG AS (SELECT * FROM Ships)


SELECT (SELECT name from passenger 
where id_psg = t.id_psg) as name, cnt
from(SELECT distinct pit.id_psg, count(*) as cnt
from pass_in_trip as pit
group by pit.id_psg, pit.place
having count(*) >= all (SELECT count(*) as cnt
from pass_in_trip as pit
group by pit.id_psg, pit.place)) as t
