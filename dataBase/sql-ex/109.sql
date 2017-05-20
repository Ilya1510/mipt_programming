WITH MyG AS (SELECT * FROM Ships),
my as (SELECT isnull(b_q_id, q_id) as id, isnull(q, 0) as q
from (SELECT b_q_id, (sum(case when c = 'r' then b_vol else 0 end) + 
sum(case when c = 'g' then b_vol else 0 end) +
sum(case when c = 'b' then b_vol else 0 end)) as q
from (SELECT *, (SELECT v_color from utv where v_id = b_v_id) as c
from utb) as t
group by b_q_id) as t1 full join utq on b_q_id = q_id)

SELECT (SELECT q_name from utq where id = q_id) as name, (SELECT count(*) from my where q = 765) as white, (SELECT count(*) from my where q = 0) as  black
from my
where q = 0 or q = 765

