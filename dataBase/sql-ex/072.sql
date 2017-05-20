WITH MyG AS (SELECT * FROM Ships)

SELECT 
(SELECT name 
from passenger 
where id_psg = pit.id_psg) as name, 
count(trip.trip_no) as cnt
from trip join pass_in_trip as pit 
        on trip.trip_no = pit.trip_no
group by pit.id_psg
HAVING count(trip.trip_no) >= 
all (SELECT count(trip. trip_no)
from trip  join pass_in_trip as pit 
on trip.trip_no = pit.trip_no
group by pit.id_psg
HAVING count (distinct trip.id_comp) = 1) 
and count(distinct trip.id_comp) = 1

