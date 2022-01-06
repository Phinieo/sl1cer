#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configs.h"



//RETURNED STRING MUST BE FREE()'D
char* makeG1(char* XYZEF, float* values){

    char *g1String = calloc(90,sizeof(char));

    strcat(g1String,"G1 ");


    for(int i = 0; i < strlen(XYZEF); i++){

        //COPY X,Y,Z,E or F
        strncat(g1String,&XYZEF[i],1);



        //CONVERT VALUES TO STRING BEFORE ADDING TO GCODE

        char temp[20];

        sprintf(temp, "%f", values[i]);

        strcat(g1String,temp);


        
        //ADD ENDING SPACE

        strcat(g1String," ");

    }

    strcat(g1String,"\n");

    return g1String;

}

void writeStart(FILE *fp){

    fputs(START_GCODES,fp);

    return;

}

void moveToStart(FILE *fp){

    float centerX = MAX_X/2;
    float centerY = MAX_Y/2;

    float startX = centerX - OBJECT_X/2;
    float startY = centerY - OBJECT_Y/2;


    //STARTS ON CENTER PERIMETER

    for(int i = 0; i < PERIMETERS; i++){

        if(i == 0){

            startX += EXTERNAL_PERIMETER_WIDTH;
            startY += EXTERNAL_PERIMETER_WIDTH;

        }else{

            startX += INTERNAL_PERIMETER_WIDTH;
            startY += INTERNAL_PERIMETER_WIDTH;

        }

    }


    startX -= (0.1075 * 0.4 * ((2 * PERIMETERS) - 2));
    startY -= (0.1075 * 0.4 * ((2 * PERIMETERS) - 2));

    if(PERIMETERS == 1){

        startX -= (EXTERNAL_PERIMETER_WIDTH/2);
        startY -= (EXTERNAL_PERIMETER_WIDTH/2);

    }else{

        startX -= (INTERNAL_PERIMETER_WIDTH/2);
        startY -= (INTERNAL_PERIMETER_WIDTH/2);

    }

    printf("%f,%f",startX,startY);


    return;

}

int main(){


    FILE *fp;

    fp = fopen(GCODE_OUT, "w");


    writeStart(fp);

//    moveToStart(fp);




    char tempXYZ[] = "XYE";

    float tempValues[] = {35.777f,38.293f,15.22440f};

    printf("%s\n",makeG1(tempXYZ,tempValues));






    fclose(fp);

}
