SELECT COALESCE(I.point, O.point), COALESCE(I.date, O.date), inc, out FROM
Income_o AS I FULL JOIN Outcome_o AS O ON
I.date = O.date AND I.point = O.point

