WITH MyG AS (SELECT * FROM Ships),
red as (SELECT distinct b_v_id
from (SELECT b_q_id
from (SELECT *, (SELECT v_color from utv where v_id = b_v_id) as c
from utb) as t
group by b_q_id
having sum(case when c = 'b' then b_vol else 0 end) > 0) as a inner join (SELECT *, (SELECT v_color from utv where v_id = b_v_id) as c
from utb) as r on a.b_q_id = r.b_q_id
where r.c = 'r')

SELECT (SELECT v_name from utv where v_id = red.b_v_id) as name
from red inner join utb on red.b_v_id = utb.b_v_id
group by red.b_v_id
having count(*) > 1

