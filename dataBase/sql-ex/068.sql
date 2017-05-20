WITH MyG AS (SELECT * FROM Ships),
t as
(select count(trip_no) as tripppps
from trip
group by case when (town_from > town_to) then town_from
else town_to end,
case when (town_from > town_to) then town_to
else town_from end
having count(trip_no) >= all (select count(trip_no)
from trip
group by case when (town_from > town_to) then town_from
else town_to end,
case when (town_from > town_to) then town_to
else  town_from end) )

select count(*)
from t

