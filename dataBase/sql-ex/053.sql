With classWond as (
	SELECT * from classes where type = 'bb'
)
SELECT cast(avg(cast(numGuns as float)) 
    as decimal(12, 2)) FROM classWond

