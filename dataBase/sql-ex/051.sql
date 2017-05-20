WITH MyG AS (SELECT * FROM Ships),

bumbumbum as (
	Select name, numGuns, displacement from classes join ships on classes.class = ships.class
	Union
	Select ship as name, numGuns, displacement from classes join outcomes on ship = class
),
blablabla as (
	Select name as nm, numGuns as ng, displacement as dsp from classes join ships on classes.class = ships.class
	Union
	Select ship as nm, numGuns as ng, displacement as dsp from classes join outcomes on ship = class
)
Select name from bumbumbum where numGuns >= all(select ng from blablabla where dsp = displacement)

