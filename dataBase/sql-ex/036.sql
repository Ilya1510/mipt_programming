    select DISTINCT Classes.
class from Classes inner join
Ships on (classes. class =
ships. name) UNION select
distinct Classes. class from
Classes inner join Outcomes on
(classes. class = Outcomes.
ship)
