

//RETURNED STRING MUST BE FREE()'D
void writeG1(char* XYZEF, float* values, FILE *fp){

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

    fputs(g1String,fp);

    free(g1String);

    return;

}














//MOVES TO START OF PERIMETER BY INDECIE. PERIMETERS ARE COUNTED FROM OUTSIDE IN

void moveToPerimeterStart(FILE *fp, int perim){

    float centerX = MAX_X/2;
    float centerY = MAX_Y/2;

    float startX = centerX - OBJECT_X/2;
    float startY = centerY - OBJECT_Y/2;


    //STARTS ON CENTER PERIMETER

    for(int i = 0; i < (PERIMETERS - perim); i++){

        if(i == 0){

            startX += EXTERNAL_PERIMETER_WIDTH;
            startY += EXTERNAL_PERIMETER_WIDTH;

        }else{

            startX += INTERNAL_PERIMETER_WIDTH;
            startY += INTERNAL_PERIMETER_WIDTH;

        }

    }


    startX -= (PERIMETER_OVERLAP * LAYER_HEIGHT * ((2 * (PERIMETERS - perim)) - 2));
    startY -= (PERIMETER_OVERLAP * LAYER_HEIGHT * ((2 * (PERIMETERS - perim)) - 2));

    if(perim == (PERIMETERS - 1)){

        startX -= (EXTERNAL_PERIMETER_WIDTH/2);
        startY -= (EXTERNAL_PERIMETER_WIDTH/2);

    }else{

        startX -= (INTERNAL_PERIMETER_WIDTH/2);
        startY -= (INTERNAL_PERIMETER_WIDTH/2);

    }



    char *tempXYZ[5];

    strcpy(tempXYZ,"XYF");

    float tempValues[] = {startX,startY,TRAVEL_SPEED};
   
    writeG1(tempXYZ,tempValues,fp);

    return;

}












void printPerimeter(FILE *fp, int currentPerimeter, float *currentZ, float *currentE){


    char tempXYZ[5];

    float tempValues[5];




    moveToPerimeterStart(fp,currentPerimeter);



    strcpy(tempXYZ,"F");

    if((*currentZ <= (LAYER_HEIGHT + 0.000001f)) || (currentPerimeter == (PERIMETERS-1))){
    
        tempValues[0] = BOTTOM_SURFACE_SPEED;
    
    }else{

        tempValues[0] = PERIMETER_SPEED; 

    }

    writeG1(tempXYZ,tempValues,fp);



 
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
    float overlap = (PERIMETER_OVERLAP * LAYER_HEIGHT * ((2*(PERIMETERS-currentPerimeter))-2));
    float summ_Outside_Perimeters = EXTERNAL_PERIMETER_WIDTH + (((PERIMETERS - currentPerimeter) - 1) * INTERNAL_PERIMETER_WIDTH);



    float perimeter_X_Low, perimeter_Y_Low, perimeter_X_High, perimeter_Y_High;




    //CHECK TO SEE IF PRINTING THE EXTERNAL PERIMETER
    
    if(currentPerimeter == (PERIMETERS - 1)){

    
        //CALCULATE FOR EXTERNAL PERIMETER    
        perimeter_X_Low = object_X_Low + (summ_Outside_Perimeters - overlap - EXTERNAL_PERIMETER_WIDTH/2); //X VALUE FOR PERIMETER LEFT BORDER
        perimeter_Y_Low = object_Y_Low + (summ_Outside_Perimeters - overlap - EXTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER BOT BORDER

    
        perimeter_X_High = object_X_High - (summ_Outside_Perimeters - overlap - EXTERNAL_PERIMETER_WIDTH/2);  //X VALUE FOR PERIMETER RIGHT BORDER
        perimeter_Y_High = object_Y_High - (summ_Outside_Perimeters - overlap - EXTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER TOP BORDER
            
    }else{
             
        //CALCULATE FOR INTERNAL PERIMETER
        perimeter_X_Low = object_X_Low + (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //X VALUE FOR PERIMETER LEFT BORDER
        perimeter_Y_Low = object_Y_Low + (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER BOT BORDER

        perimeter_X_High = object_X_High - (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2);  //X VALUE FOR PERIMETER RIGHT BORDER
        perimeter_Y_High = object_Y_High - (summ_Outside_Perimeters - overlap - INTERNAL_PERIMETER_WIDTH/2); //Y VALUE FOR PERIMETER TOP BORDER
            

    }





            
    //CALCULATE EXTRUSIONS FOR CURRENT PERIMETER
    float extrusion_X = ((((INTERNAL_PERIMETER_WIDTH-LAYER_HEIGHT)*LAYER_HEIGHT)+(3.1415926*((LAYER_HEIGHT/2)*(LAYER_HEIGHT/2))))*(perimeter_X_High - perimeter_X_Low))/(3.1415926*((FILAMENT_DIAMETER/2)*(FILAMENT_DIAMETER/2)));

    float extrusion_Y = ((((INTERNAL_PERIMETER_WIDTH-LAYER_HEIGHT)*LAYER_HEIGHT)+(3.1415926*((LAYER_HEIGHT/2)*(LAYER_HEIGHT/2))))*(perimeter_Y_High - perimeter_Y_Low))/(3.1415926*((FILAMENT_DIAMETER/2)*(FILAMENT_DIAMETER/2)));




    //DO ALL 4 CORNERS GOING IN ORDER OF TOP-DOWN CORNERS: BOT L -> BOT R -> TOP R -> TOP L -> BOT L

    strcpy(tempXYZ,"XYE");



    //MOVE TO BOT R

    tempValues[0] = perimeter_X_High;
    tempValues[1] = perimeter_Y_Low;

    *currentE += extrusion_X;
    tempValues[2] = *currentE;

    writeG1(tempXYZ,tempValues,fp);



    //MOVE TO TOP R

    tempValues[0] = perimeter_X_High;
    tempValues[1] = perimeter_Y_High;

    *currentE += extrusion_Y;
    tempValues[2] = *currentE;

    writeG1(tempXYZ,tempValues,fp);



    //MOVE TO TOP L

    tempValues[0] = perimeter_X_Low;
    tempValues[1] = perimeter_Y_High;

    *currentE += extrusion_X;
    tempValues[2] = *currentE;

    writeG1(tempXYZ,tempValues,fp);



    //MOVE TO BOT L
    //DOES NOT QUITE COMPLETE LOOP SO AS NOT TO OVEREXTRUDE AT STARTING POINT
    //USING PERIMETER_OVERLAP BUT EXAMPLE GCODE SHOWS FACTOR OF 15% OF LAYER_HEIGHT

    tempValues[0] = perimeter_X_Low;
    tempValues[1] = perimeter_Y_Low + (PERIMETER_OVERLAP * INTERNAL_PERIMETER_WIDTH);

    *currentE += extrusion_Y;
    tempValues[2] = *currentE;

    writeG1(tempXYZ,tempValues,fp);




    fputs("\n",fp);






    return;


}
















//MODIFIED PERIMETER GCODE GENERATION
//ALL ITERATION TO Z AXIS IS DONE HERE
void printAllPerimeters(FILE *fp, float *currentZ, float *currentE){


    char tempXYZ[5];

    float tempValues[5];






    //DO OFFSET PERIMETERS FIRST

    //ITERATE Z BY HALF LAYER
    strcpy(tempXYZ,"ZF");


    *currentZ += LAYER_HEIGHT/2;
    tempValues[0] = *currentZ;
    tempValues[1] = TRAVEL_SPEED;

    writeG1(tempXYZ,tempValues,fp);



    //PERIMETER 0 = INSIDE. PRINTS FROM INSIDE PERIMETERS TO OUTSIDE PERIMETERS
    for(int i = 0; i < PERIMETERS; i++){

        //IF # PERIMETERS IS ODD THEN ONLY ODD PERIMETERS ARE OFFSETS
        if((PERIMETERS % 2 == 1) && (i % 2 == 1)){


            printPerimeter(fp, i, currentZ, currentE);


        }

        //IF # PERIMETERS IS EVEN THEN ONLY EVEN PERIMETERS ARE OFFSETS
        if((PERIMETERS % 2 == 0) && (i % 2 == 0)){


            printPerimeter(fp, i, currentZ, currentE);


        }

    }







    fputs("\n\n",fp);










    //DO REGULAR HEIGHT PERIMETERS

    //ITERATE Z BY HALF LAYER
    strcpy(tempXYZ,"ZF");


    *currentZ += LAYER_HEIGHT/2;
    tempValues[0] = *currentZ;
    tempValues[1] = TRAVEL_SPEED;

    writeG1(tempXYZ,tempValues,fp);





    //PERIMETER 0 = INSIDE. PRINTS FROM INSIDE PERIMETERS TO OUTSIDE PERIMETERS
    for(int i = 0; i < PERIMETERS; i++){

        //IF # PERIMETERS IS ODD THEN ONLY EVEN PERIMETERS ARE REGULAR HEIGHT
        if((PERIMETERS % 2 == 1) && (i % 2 == 0)){


            printPerimeter(fp, i, currentZ, currentE);


        }

        //IF # PERIMETERS IS EVEN THEN ONLY EVEN PERIMETERS ARE OFFSETS
        if((PERIMETERS % 2 == 0) && (i % 2 == 1)){


            printPerimeter(fp, i, currentZ, currentE);


        }

    }


    fputs("\n\n\n",fp);


    return;

}





void writeEnd(FILE *fp){

    fputs(END_GCODES,fp);

    return;

}







void writeStart(FILE *fp){

    fputs(START_GCODES,fp);

    return;

}








