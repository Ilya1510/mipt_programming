Select name FROM 
Ships INNER JOIN Classes ON
Ships.class = Classes.class AND launched >= 1922 AND type = 'bb' AND displacement > 35000

