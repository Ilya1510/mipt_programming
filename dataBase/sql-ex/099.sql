WITH MyG AS (SELECT * FROM Ships),
temp as(SELECT isnull(i.point, o.point) as point, isnull(i.date, o.date) as date, i.inc, o.out
from income_o as i full join outcome_o as o on i.date = o.date and i.point = o.point)

SELECT t.point, t.date as dp, case when t.out is null then t.date else

(SELECT min(day) as min_date
from
(SELECT dateadd(dd, 1, t.date) as day
union all
SELECT dateadd(dd, 2, t.date)
union all
SELECT dateadd(dd, 3, t.date)
union all
SELECT dateadd(dd, 1, (SELECT min(temp.date) from temp where date > t.date and dateadd(dd, 1, temp.date) not in (SELECT date from outcome_o where point = t.point) and datename(weekday, dateadd(dd, 1, date)) <> 'sunday'))
) as dat
where (dat.day not in (SELECT date from outcome_o where point = t.point)) and (datename(weekday, day) <> 'sunday'))

end as di
from temp as t
where t.inc is not null

