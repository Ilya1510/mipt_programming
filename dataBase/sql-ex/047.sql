with peoples as (
	select maker as mk, count(maker) as cnt FROM product GROUP BY maker
)
select row_number() over (order by cnt desc, maker, model) no, maker, model 
FROM peoples join product on peoples.mk = product.maker order by model

