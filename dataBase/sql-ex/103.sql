WITH MyG AS (SELECT * FROM Ships)


SELECT (SELECT min(trip_no) from trip) as min1,
(SELECT min(trip_no) from trip where trip_no > (SELECT min(trip_no) from trip)) as min2,
(SELECT min(trip_no) from trip where trip_no > (SELECT min(trip_no) from trip where trip_no > (SELECT min(trip_no) from trip))) as min3,
(SELECT max(trip_no) from trip where trip_no < (SELECT max(trip_no) from trip where trip_no < (SELECT max(trip_no) from trip))) as max3,
(SELECT max(trip_no) from trip where trip_no < (SELECT max(trip_no) from trip)) as max2,
(SELECT max(trip_no) from trip) as max1

