WITH MyG AS (SELECT * FROM Ships)


SELECT (SELECT name from passenger where  id_psg = p.id_psg)
from (SELECT pit.id_psg, trip.town_from as town
from pass_in_trip as pit inner join trip on pit.trip_no = trip.trip_no
group by pit.id_psg, trip.town_from

union

SELECT pit.id_psg, trip.town_to
from pass_in_trip as pit inner join trip on pit.trip_no = trip.trip_no
group by pit.id_psg, trip.town_to) as x inner join passenger as p on x.id_psg = p.id_psg
group by p.id_psg
having count(distinct town) = 2

