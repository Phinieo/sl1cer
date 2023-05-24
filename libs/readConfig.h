////////
//FILE//
////////

extern char* STL_IN;
extern char* GCODE_OUT;


/////////////////////
//START & END CODES//
/////////////////////

extern char* START_GCODES;

extern char* END_GCODES;



/////////////////////
//HARDWARE SETTINGS//
/////////////////////

extern float NOZZLE;

extern float MAX_X;
extern float MAX_Y;
extern float MAX_Z;




/////////////////////
//FILAMENT SETTINGS//
/////////////////////

extern float NOZZLE_TEMP;
extern float BED_TEMP;

extern float FILAMENT_DIAMETER;



//////////////////////
//RESOLUTION & WIDTH//
//////////////////////

extern float LAYER_HEIGHT;

extern float PERIMETERS;

//SPACE BETWEEN INFILL PATTERN IN mm
//A NEGATIVE VALUE GIVES NO INFILL
//MAX VALUE OF 10 mm
extern float INFILL_SPACING;

extern float EXTRUSION_WIDTH;

extern float EXTERNAL_PERIMETER_WIDTH;
extern float INTERNAL_PERIMETER_WIDTH;

extern float INFILL_WIDTH;

extern float PERIMETER_OVERLAP;



extern float RETRACTION_DISTANCE;



//////////
//SPEEDS//
//////////

extern float TRAVEL_SPEED;
extern float PERIMETER_SPEED;
extern float INFILL_SPEED;
extern float EXTERNAL_PERIMETER_SPEED;

extern float TOP_SURFACE_SPEED;
extern float TOP_INTERNAL_SPEED;

extern float BOTTOM_SURFACE_SPEED;


int readConfig(char* fileName);


