#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libs/readConfig.h"

////////
//FILE//
////////

char* STL_IN;
char* GCODE_OUT;


/////////////////////
//START & END CODES//
/////////////////////

char* START_GCODES;

char* END_GCODES;



/////////////////////
//HARDWARE SETTINGS//
/////////////////////

float NOZZLE;

float MAX_X;
float MAX_Y;
float MAX_Z;




/////////////////////
//FILAMENT SETTINGS//
/////////////////////

float NOZZLE_TEMP;
float BED_TEMP;

float FILAMENT_DIAMETER;



//////////////////////
//RESOLUTION & WIDTH//
//////////////////////

float LAYER_HEIGHT;

float PERIMETERS;

//SPACE BETWEEN INFILL PATTERN IN mm
//A NEGATIVE VALUE GIVES NO INFILL
//MAX VALUE OF 10 mm
float INFILL_SPACING;

float EXTRUSION_WIDTH;

float EXTERNAL_PERIMETER_WIDTH;
float INTERNAL_PERIMETER_WIDTH;

float INFILL_WIDTH;

float PERIMETER_OVERLAP;



float RETRACTION_DISTANCE;



//////////
//SPEEDS//
//////////

float TRAVEL_SPEED;
float PERIMETER_SPEED;
float INFILL_SPEED;
float EXTERNAL_PERIMETER_SPEED;

float TOP_SURFACE_SPEED;
float TOP_INTERNAL_SPEED;

float BOTTOM_SURFACE_SPEED;


int readConfig(char* fileName){


    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        // Handle file open error
        printf("Failed to open the file.\n");
        return 0;
    }

    char line[500];
    while (fgets(line, sizeof(line), file)) {

        printf("STL_IN VALUE LOOP UPDATE: %s\n",STL_IN);

        char *configType = strtok(line, " ");

        if(strcmp(configType, "STL_IN") == 0){

            configType = strtok(NULL,"\n");
            STL_IN = calloc(strlen(configType),sizeof(char));
            strcpy(STL_IN,configType);
            printf("ASSIGNED STL_IN: %s\n",STL_IN);
            continue;

        }else if(strcmp(configType, "GCODE_OUT") == 0){

            configType = strtok(NULL,"\n");
            GCODE_OUT = calloc(strlen(configType),sizeof(char));
            strcpy(GCODE_OUT,configType);
            printf("ASSIGNED GCODE_OUT: %s\n",GCODE_OUT);
            continue;

        }else if(strcmp(configType, "START_GCODES") == 0){

            configType = strtok(NULL,"");
            START_GCODES = calloc(strlen(configType),sizeof(char));
            strcpy(START_GCODES,configType);
            printf("ASSIGNED START_GCODES: %s\n",START_GCODES);
            continue;

        }else if(strcmp(configType, "END_GCODES") == 0){

            configType = strtok(NULL,"");
            END_GCODES = calloc(strlen(configType),sizeof(char));
            strcpy(END_GCODES,configType);
            printf("ASSIGNED END_GCODES: %s\n",END_GCODES);
            continue;

        }else if(strcmp(configType, "NOZZLE") == 0){

            configType = strtok(NULL," ");
            NOZZLE = (float)atof(configType);
            printf("ASSIGNED NOZZLE: %f\n",NOZZLE);
            continue;

        }else if(strcmp(configType, "MAX_X") == 0){

            configType = strtok(NULL," ");
            MAX_X = (float)atof(configType);
            printf("ASSIGNED MAX_X: %f\n",MAX_X);
            continue;

        }else if(strcmp(configType, "MAX_Y") == 0){

            configType = strtok(NULL," ");
            MAX_Y = (float)atof(configType);
            printf("ASSIGNED MAX_Y: %f\n",MAX_Y);
            continue;

        }else if(strcmp(configType, "MAX_Z") == 0){

            configType = strtok(NULL," ");
            MAX_Z = (float)atof(configType);
            printf("ASSIGNED MAX_Z: %f\n",MAX_Z);
            continue;

        }else if(strcmp(configType, "NOZZLE_TEMP") == 0){

            configType = strtok(NULL," ");
            NOZZLE_TEMP = (float)atof(configType);
            printf("ASSIGNED NOZZLE_TEMP: %f\n",NOZZLE_TEMP);
            continue;

        }else if(strcmp(configType, "BED_TEMP") == 0){

            configType = strtok(NULL," ");
            BED_TEMP = (float)atof(configType);
            printf("ASSIGNED BED_TEMP: %f\n",BED_TEMP);
            continue;

        }else if(strcmp(configType, "FILAMENT_DIAMETER") == 0){

            configType = strtok(NULL," ");
            FILAMENT_DIAMETER = (float)atof(configType);
            printf("ASSIGNED FILAMENT_DIAMETER: %f\n",FILAMENT_DIAMETER);
            continue;

        }else if(strcmp(configType, "LAYER_HEIGHT") == 0){

            configType = strtok(NULL," ");
            LAYER_HEIGHT = (float)atof(configType);
            printf("ASSIGNED LAYER_HEIGHT: %f\n",LAYER_HEIGHT);
            continue;

        }else if(strcmp(configType, "PERIMETERS") == 0){

            configType = strtok(NULL," ");
            PERIMETERS = (float)atof(configType);
            printf("ASSIGNED PERIMETERS: %f\n",PERIMETERS);
            continue;

        }else if(strcmp(configType, "INFILL_SPACING") == 0){

            configType = strtok(NULL," ");
            INFILL_SPACING = (float)atof(configType);
            printf("ASSIGNED INFILL_SPACING: %f\n",INFILL_SPACING);
            continue;

        }else if(strcmp(configType, "PERIMETER_OVERLAP") == 0){

            configType = strtok(NULL," ");
            PERIMETER_OVERLAP = (float)atof(configType);
            printf("ASSIGNED PERIMETER_OVERLAP: %f\n",PERIMETER_OVERLAP);
            continue;

        }else if(strcmp(configType, "RETRACTION_DISTANCE") == 0){

            configType = strtok(NULL," ");
            RETRACTION_DISTANCE = (float)atof(configType);
            printf("ASSIGNED RETRACTION_DISTANCE: %f\n",RETRACTION_DISTANCE);
            continue;

        }else if(strcmp(configType, "TRAVEL_SPEED") == 0){

            configType = strtok(NULL," ");
            TRAVEL_SPEED = (float)atof(configType);
            printf("ASSIGNED TRAVEL_SPEED: %f\n",TRAVEL_SPEED);
            continue;

        }else if(strcmp(configType, "PERIMETER_SPEED") == 0){

            configType = strtok(NULL," ");
            PERIMETER_SPEED = (float)atof(configType);
            printf("ASSIGNED PERIMETER_SPEED: %f\n",PERIMETER_SPEED);
            continue;

        }else if(strcmp(configType, "INFILL_SPEED") == 0){

            configType = strtok(NULL," ");
            INFILL_SPEED = (float)atof(configType);
            printf("ASSIGNED INFILL_SPEED: %f\n",INFILL_SPEED);
            continue;

        }else if(strcmp(configType, "EXTERNAL_PERIMETER_SPEED") == 0){

            configType = strtok(NULL," ");
            EXTERNAL_PERIMETER_SPEED = (float)atof(configType);
            printf("ASSIGNED EXTERNAL_PERIMETER_SPEED: %f\n",EXTERNAL_PERIMETER_SPEED);
            continue;

        }else if(strcmp(configType, "TOP_SURFACE_SPEED") == 0){

            configType = strtok(NULL," ");
            TOP_SURFACE_SPEED = (float)atof(configType);
            printf("ASSIGNED TOP_SURFACE_SPEED: %f\n",TOP_SURFACE_SPEED);
            continue;

        }else if(strcmp(configType, "TOP_INTERNAL_SPEED") == 0){

            configType = strtok(NULL," ");
            TOP_INTERNAL_SPEED = (float)atof(configType);
            printf("ASSIGNED TOP_INTERNAL_SPEED: %f\n",TOP_INTERNAL_SPEED);
            continue;

        }else if(strcmp(configType, "BOTTOM_SURFACE_SPEED") == 0){

            configType = strtok(NULL," ");
            BOTTOM_SURFACE_SPEED = (float)atof(configType);
            printf("ASSIGNED BOTTOM_SURFACE_SPEED: %f\n",BOTTOM_SURFACE_SPEED);
            continue;

        }else{

            //printf("UNKNOWN CONFIGURATION TYPE READ: %s\n",configType);
            continue;

        }

    }

    EXTRUSION_WIDTH = (NOZZLE * 1.2);

    EXTERNAL_PERIMETER_WIDTH = (NOZZLE * 1.1);
    INTERNAL_PERIMETER_WIDTH = (NOZZLE * 1.05);

    INFILL_WIDTH = (NOZZLE * 1.0);

    fclose(file);
    
    return 1;
}

