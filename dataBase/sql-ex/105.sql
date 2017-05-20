WITH MyG AS (SELECT * FROM Ships)


SELECT maker, model, row_number() over(order by maker, model) as a,

(SELECT q
from (SELECT row_number() over(order by maker) as q, maker
from product group by maker) as t1
where maker = p.maker) as b,

(SELECT min(q)
from (SELECT row_number() over(order by maker, model) as q, maker
from product) as t2
where maker = p.maker) as c,

(SELECT max(q)
from (SELECT row_number() over(order by maker, model) as q, maker
from product) as t3
where maker = p.maker) as d

from product as p

