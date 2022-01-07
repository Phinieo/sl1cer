M107
M190 S65 ; set bed temperature and wait for it to be reached
M104 S215 ; set temperature
G28 ; home all axes
G1 Z5 F5000 ; lift nozzlen
M109 S215 ; set temperature and wait for it to be reached
G21 ; set units to millimeters
G90 ; use absolute coordinates
M82 ; use absolute distances for extrusion
G92 E0
G1 E-2.00000 F2400.00000
G92 E0


G1 X81.565994 Y81.565994 F7800.000000 


G1 E2.00000 F2400.00000

G1 Z0.200000 F7800.000000 0.000000 
