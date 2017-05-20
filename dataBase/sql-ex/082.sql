WITH MyG AS (SELECT * FROM Ships),
NPCCC as(select row_number() over(order by code) as no, code, price
from pc)

select npc.code, (select avg(price) from NPCCC where no >= npc.no and no-5 <= npc.no ) as avgrpc
from NPCCC as npc
where npc.no + 4 < (select count(*) from pc)

