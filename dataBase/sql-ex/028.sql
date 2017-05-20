SELECT CASE WHEN one is NULL THEN 0 ELSE CAST(1.0 * one/two AS Numeric(30, 2)) END
FROM (Select SUM(B_VOL) FROM utB) AS A(one), 
(Select COUNT(Q_ID) FROM utQ) AS B(two)

