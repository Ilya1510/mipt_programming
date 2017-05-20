SELECT DISTINCT a.model, b.model, a.speed, a.ram FROM
PC AS a, PC AS b WHERE
a.model > b.model AND a.speed=b.speed AND a.ram=b.ram

