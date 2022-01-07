#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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






//MODIFIED PERIMETER GCODE GENERATION
//ALL ITERATION TO Z AXIS IS DONE HERE
void printModPerimeter(FILE *fp, float *currentZ, float *currentE){


    char tempXYZ[5];

    float tempValues[5];

    char *G1Line;
 


    //DO OFFSET PERIMETERS FIRST

    //ITERATE Z BY HALF LAYER
    tempXYZ[0] = 'Z';
    tempXYZ[1] = 'F';

    *currentZ += LAYER_HEIGHT/2;
    tempValues[0] = *currentZ;
    tempValues[1] = TRAVEL_SPEED;

    G1Line = makeG1(tempXYZ,tempValues);

    fputs(G1Line,fp);

    free(G1Line);


    //PERIMETER 0 = INSIDE. PRINTS FROM INSIDE PERIMETERS TO OUTSIDE PERIMETERS
    for(int i = 0; i < PERIMETERS; i++){

        //IF # PERIMETERS IS ODD THEN ONLY ODD PERIMETERS ARE OFFSETS
        if((PERIMETERS % 2 == 1) && (i % 2 == 1)){

            //DO ALL 4 CORNERS GOING IN ORDER OF TOP-DOWN CORNERS: BOT L -> BOT R -> TOP R -> TOP L -> BOT L
 
            float centerX = MAX_X/2;
            float centerY = MAX_Y/2;

            float object_X_Low = centerX - OBJECT_X/2; //X VALUE FOR OBJECT LEFT BORDER
            float object_Y_Low = centerY - OBJECT_Y/2; //Y VALUE FOR OBJECT BOT BORDER

            float object_X_High = centerX + OBJECT_X/2; //X VALUE FOR OBJECT RIGHT BORDER
            float object_Y_High = centerY + OBJECT_Y/2; //Y VALUE FOR OBJECT TOP BORDER





            //OVERLAP MAY NEED TWEAKING
            //ADJUSTS DISTANCES BETWEEN PERIMETERS
            //MULTIPLIED BY LAYER_HEIGHT BUT MAY BE NOZZLE_DIAMETER INSTEAD?
            //# OVERLAPS=((2*X)-2) WHERE X IS # PERIMETERS FROM EXTERNAL TO CURRENT INCLUSIVE 
            float overlap = (PERIMETER_OVERLAP * LAYER_HEIGHT * ((2*(PERIMETERS-i))-2));
            float summ_Outside_Perimeters = EXTERNAL_PERIMETER_WIDTH + (((PERIMETERS - i) - 1) * INTERNAL_PERIMETER_WIDTH);


            float perimeter_X_Low = object_X_Low + (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //X VALUE FOR PERIMETER LEFT BORDER
            float perimeter_Y_Low = object_Y_Low + (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER BOT BORDER

            float perimeter_X_High = object_X_High - (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2);  //X VALUE FOR PERIMETER RIGHT BORDER
            float perimeter_Y_High = object_Y_High - (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER TOP BORDER


            printf("%f\n%f\n%f\n%f\n\n",perimeter_X_Low,perimeter_Y_Low,perimeter_X_High,perimeter_Y_High);

           
            


        }

        //IF # PERIMETERS IS EVEN THEN ONLY EVEN PERIMETERS ARE OFFSETS
        if((PERIMETERS % 2 == 0) && (i % 2 == 0)){

        
            


        }

    }

    return;

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



    char tempXYZ[] = "XYF";

    float tempValues[] = {startX,startY,TRAVEL_SPEED};
   
    char *temp =  makeG1(tempXYZ,tempValues);

    fputs(temp,fp);

    free(temp);

    return;

}


