WITH MyG AS (SELECT * FROM Ships),
XXX as (select point, date, sum(inc-out) as r
from (select point, date, sum(coalesce(inc, 0)) as inc, 0 as out
from income
group by point, date
union
select point, date, 0, sum(coalesce(out, 0))
from outcome
group by point, date) as t
group by point, date)

select point, 
convert(char(10), date, 103), 
(select sum(r) 
from XXX 
where (date <= z.date) 
and (point = z.point)) as r
from XXX as z

