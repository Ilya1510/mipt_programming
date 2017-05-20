WITH MyG AS (SELECT * FROM Ships),
notblack as (SELECT b_q_id, 
sum(case when c = 'r' then b_vol else 0 end) as r,
sum(case when c = 'g' then b_vol else 0 end) as g,
sum(case when c = 'b' then b_vol else 0 end) as b
from (SELECT *, (SELECT v_color from utv where v_id = b_v_id) as c
from utb) as t
group by b_q_id)

SELECT
255*((SELECT count(*) from utq) - (SELECT count(*) from notblack)) + sum(255 - r) as r,
255*((SELECT count(*) from utq) - (SELECT count(*) from notblack)) +  sum(255 - g) as g,
255*((SELECT count(*) from utq) - (SELECT count(*) from notblack)) + sum(255 - b) as b
from notblack

