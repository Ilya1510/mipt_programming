WITH MyG AS (SELECT * FROM Ships)


SELECT date_begin, date_finish
from (SELECT row_number() over (order by date_begin) as n, *
from (SELECT distinct b_datetime as date_begin
from utb
where dateadd(s, -1, b_datetime) not in (SELECT b_datetime from utb) 
      and dateadd(s, 1, b_datetime) 
          in (SELECT b_datetime from utb)) as start) as t1 
    inner join (SELECT row_number() over (order by date_finish) as n, *
from (SELECT distinct b_datetime as date_finish
from utb
where dateadd(s, 1, b_datetime) not in (SELECT b_datetime from utb) 
      and dateadd(s, -1, b_datetime) in (SELECT b_datetime from utb)) as finish) as t2 on t1.n = t2.n
