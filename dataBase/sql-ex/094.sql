WITH MyG AS (SELEct * FROM Ships),
mind as (SELEct min(date) as date
from (SELEct pit.date, count(distinct trip.trip_no) as trip
from pass_in_trip as pit inner join trip on trip.trip_no = pit.trip_no
where town_from = 'rostov' 
group by pit.date
having count(distinct trip.trip_no) >= all (SELEct count(distinct trip.trip_no)
from pass_in_trip as pit2 inner join trip on trip.trip_no = pit2.trip_no
where town_from = 'rostov'
group by pit2.date)) as e)

SELEct week.date, case when week.date not in (SELEct date from pass_in_trip as pt inner join trip on trip.trip_no = pt.trip_no where trip.town_from = 'rostov') then 0 else (SELEct count (distinct pt.trip_no) from trip inner join pass_in_trip as pt on pt.trip_no = trip.trip_no where trip.town_from = 'rostov' and pt.date = week.date) end as cnt
from (SELEct date from mind
union SELEct dateadd(dd, 1, date) from mind
union SELEct dateadd(dd, 2, date) from mind
union SELEct dateadd(dd, 3, date) from mind
union SELEct dateadd(dd, 4, date) from mind
union SELEct dateadd(dd, 5, date) from mind
union SELEct dateadd(dd, 6, date) from mind) as week

