WITH MyG AS (SELECT * FROM Ships),
s as (select count(model) as c
from product
group by maker)
select maker, count(model)
from product
group by maker
having count(model) >= all (select * from s)
union
select maker, count(model)
from product
group by maker
having count(model) <= all (select * from s)


