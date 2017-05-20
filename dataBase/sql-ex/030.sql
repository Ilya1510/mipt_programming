SELECT COALESCE(I.point, O.point), COALESCE(I.date, O.date), SUM(out), SUM(inc) FROM
(SELECT point, date, SUM(inc) AS inc FROM Income 
Group By point, date) AS I FULL JOIN 
(SELECT point, date, SUM(out) AS out FROM Outcome 
Group By point, date) AS O ON
I.date = O.date AND I.point = O.point
Group by COALESCE(I.point, O.point), COALESCE(I.date, O.date)
ORDER BY COALESCE(I.date, O.date)

