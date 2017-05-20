WITH max_pc as (
	select CAST(pc.code as VARCHAR(15)) as code,
	CAST(pc.model as VARCHAR(15)) as model,
	CAST(pc.speed as VARCHAR(15)) as speed,
	CAST(pc.ram as VARCHAR(15)) as ram,
	CAST(pc.hd as VARCHAR(15)) as hd,
	CAST(pc.cd as VARCHAR(15)) as cd,
	CAST(pc.price as VARCHAR(15)) as price
	from pc where code in (select max(code) from pc)
)

SELECT chr, value from max_pc
unpivot (value for chr in (model, speed, ram, hd, cd, price)) AS unpvt

