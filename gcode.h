

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







void writeEnd(FILE *fp){

    fputs(END_GCODES,fp);

    return;

}







void writeStart(FILE *fp){

    fputs(START_GCODES,fp);

    return;

}




void layerUp(struct point* currentPoint, FILE *fp){

   (*currentPoint).Z += LAYER_HEIGHT;

   writeG1("ZF",(float[2]){(*currentPoint).Z,TRAVEL_SPEED},fp);

   return;

}





void writeLayerPerim(struct edge* layerEdges, int* layerEdgesI, struct point* currentPoint, float* currentExtrusion, FILE *fp){


   //MOVE EDGES TO ABSOLUTE POINTS ON PRINTBED

   for(int i = 0; i < *layerEdgesI; i++){

      layerEdges[i] = centerEdge(layerEdges[i]);

   }

     
   //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

   // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


   float lowestDistance = 0;

   int startEdge = 0;

   for(int i = 0; i < *layerEdgesI; i++){

      if(pointDistance(layerEdges[i].p1, *currentPoint) < lowestDistance || i == 0){

         lowestDistance = pointDistance(layerEdges[i].p1, *currentPoint);

         startEdge = i;

      }

   }



   //GCODE WRITE
   //MOVE TO NEAREST POINT ON THIS LAYER

   (*currentPoint).X = layerEdges[startEdge].p1.X;
   (*currentPoint).Y = layerEdges[startEdge].p1.Y;

   writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

   writeG1("XY", (float[2]){layerEdges[startEdge].p1.X, layerEdges[startEdge].p1.Y}, fp);






   //WRITE PERIMETER EDGES

   writeG1("F", (float[1]){EXTERNAL_PERIMETER_SPEED}, fp);


   int numEdgesUsed = 0;

   int edgesUsed[*layerEdgesI];

   while(numEdgesUsed < *layerEdgesI){

      for(int i = 0; i < *layerEdgesI; i++){

         if((numEdgesUsed < *layerEdgesI) && layerEdges[i].p1.X == (*currentPoint).X && layerEdges[i].p1.Y == (*currentPoint).Y){



            float extrudeDist = perimeterExtrusion(pointDistance(layerEdges[i].p2, *currentPoint));

            (*currentExtrusion) += extrudeDist;

            (*currentPoint).X = layerEdges[i].p2.X;
            (*currentPoint).Y = layerEdges[i].p2.Y;


            writeG1("XYE", (float[3]){layerEdges[i].p2.X, layerEdges[i].p2.Y,*currentExtrusion}, fp);

            
            edgesUsed[numEdgesUsed] = i;

            numEdgesUsed++;



         }

      }  


      //NO EDGE TO CONTINUE LOOP ON
      //FIND AND MOVE TO NEW LOOP



      for(int i2 = 0; i2 < *layerEdgesI; i2++){

         for(int i3 = 0; i3 < numEdgesUsed; i3++){

            if(i2 == edgesUsed[i3]){

               //EDGE IS USED
               break;

            }
               
            if(!(i3+1 < numEdgesUsed)){

               //CURRENT INDECIE NOT FOUND IN LIST. MOVE TO NEW EDGE

               (*currentPoint).X = layerEdges[i2].p1.X;
               (*currentPoint).Y = layerEdges[i2].p1.Y;

               writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

               writeG1("XY", (float[2]){layerEdges[i2].p1.X, layerEdges[i2].p1.Y}, fp);



               //NEED TO BREAK TWICE. JUST GONNA GO TO END-LOOP CONDITIONS

               i3 = numEdgesUsed;
               i2 = *layerEdgesI;


            }

         }

      }



   }






   return;

}






