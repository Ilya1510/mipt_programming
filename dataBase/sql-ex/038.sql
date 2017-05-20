SELECT country FROM Classes
WHERE type = 'bb'
INTERSECT
SELECT country FROM Classes
WHERE type='bc'

