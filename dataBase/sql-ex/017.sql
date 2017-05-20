SELECT DISTINCT 'Laptop', model, speed FROM Laptop
Where speed < ALL (SELECT speed FROM PC)

