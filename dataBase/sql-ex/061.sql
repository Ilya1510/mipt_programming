WITH MyG AS (SELECT * FROM Ships),
sum_inc as (
select sum(inc) as sum_inc 
from income_o),
sum_out as (
select sum(out) as sum_out 
from outcome_o),
NSI as (
select 0 as NSI from sum_inc 
where sum_inc is null
union
select sum_inc as NSI from sum_inc 
where sum_inc is not null),
NSO as (
select 0 as NSO from sum_out 
where sum_out is null
union
select sum_out as NSO from sum_out 
where sum_out is not null)
select NSI - NSO from NSI 
    join NSO on 2 = 2

