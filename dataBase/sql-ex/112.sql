WITH MyG AS (SELECT * FROM Ships),
notblack as (SELECT 
(SELECT 255*count(*) from utv where v_color = 'r') - sum(case when c = 'r' then b_vol else 0 end) as r,
(SELECT 255*count(*) from utv where v_color = 'g') - sum(case when c = 'g' then b_vol else 0 end) as g,
(SELECT 255*count(*) from utv where v_color = 'b') - sum(case when c = 'b' then b_vol else 0 end) as b
from (SELECT *, (SELECT v_color from utv where v_id = b_v_id) as c
from utb) as t)

SELECT min(c) / 255 as c
from (SELECT r as c
from notblack
union all
SELECT g
from notblack
union all
SELECT b
from notblack) as y

