////////
//FILE//
////////

#define STL_IN "20MMCube.stl"
#define GCODE_OUT "20MMCubeGEN.gcode"

#define OBJECT_X 20
#define OBJECT_Y 20
#define OBJECT_Z 80






//////////////////
//START-UP CODES//
//////////////////

#define START_GCODES "M107\nM190 S65 ; set bed temperature and wait for it to be reached\nM104 S215 ; set temperature\nG28 ; home all axes\nG1 Z5 F5000 ; lift nozzlen\nM109 S215 ; set temperature and wait for it to be reached\nG21 ; set units to millimeters\nG90 ; use absolute coordinates\nM82 ; use absolute distances for extrusion\nG92 E0\nG1 E-2.00000 F2400.00000\nG92 E0\n"
 




/////////////////////
//HARDWARE SETTINGS//
/////////////////////

#define NOZZLE 0.4

#define MAX_X 180
#define MAX_Y 180
#define MAX_Z 200







/////////////////////
//FILAMENT SETTINGS//
/////////////////////

#define NOZZLE_TEMP 215
#define BED_TEMP 60

#define FILAMENT_DIAMETER 1.75







//////////////////////
//RESOLUTION & WIDTH//
//////////////////////

#define LAYER_HEIGHT 0.4

#define PERIMETERS 3

#define EXTRUSION_WIDTH_MULTIPLIER 1.0

#define EXTERNAL_PERIMETER_WIDTH (LAYER_HEIGHT * 1.1)
#define INTERNAL_PERIMETER_WIDTH (LAYER_HEIGHT * 1.05)

#define PERIMETER_OVERLAP 0.1075






//////////
//SPEEDS//
//////////

#define TRAVEL_SPEED 7800
#define PERIMETER_SPEED 3600
#define INFILL_SPEED 4800
#define EXTERNAL_PERIMETER_SPEED (PERIMETER_SPEED/2)

#define TOP_SURFACE_SPEED 900
#define TOP_INTERNAL_SPEED 3600

#define BOTTOM_SURFACE_SPEED 1800







