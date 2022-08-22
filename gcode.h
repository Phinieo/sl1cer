

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





void writeLayerPerim(struct edge* layerEdges, int layerEdgesI, struct point* currentPoint, float* currentExtrusion, FILE *fp){


   //MOVE EDGES TO ABSOLUTE POINTS ON PRINTBED

   for(int i = 0; i < layerEdgesI; i++){

      layerEdges[i] = centerEdge(layerEdges[i]);

   }

     
   //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

   // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


   float lowestDistance = -1;

   int startEdge = 0;

   for(int i = 0; i < layerEdgesI; i++){

      if(pointDistance(layerEdges[i].p1, *currentPoint) < lowestDistance || lowestDistance < 0){

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





   /////////////////////////////
   //SEPARATE EDGES INTO LOOPS//
   /////////////////////////////

   //List of loops which consist of lists of edges
   //Max number of loops is the number of edges divided by 3 because a loop must be at least 3 edges
   struct edge loops[layerEdgesI/3][layerEdgesI];

   int edgesPerLoop[layerEdgesI];
   int currentLoop = 0;
   int loopI = 0;


   //List of used edges from layerEdges
   int edgesUsed[layerEdgesI];

   int numEdgesUsed = 0;





   while(numEdgesUsed < layerEdgesI){
      
      printf("layerEdgesI: %d, numEdesUsed: %d, currentLoop: %d, loopI: %d\n",layerEdgesI,numEdgesUsed,currentLoop,loopI);

      int edgeAddedThisLoop = 0;

      for(int i = 0; i < layerEdgesI; i++){


         //CHECK IF CURRENTLY CONSIDERED EDGE IS ALREADY USED
         int edgeIsUsed = 0;

         for(int i2 = 0; i2 < numEdgesUsed; i2++){

            if(i == edgesUsed[i2]){

               edgeIsUsed = 1;
               printf("EDGE USED\n");
            }

         }



         //CHECK IF EDGE STARTS WITH OUR CURRENT POINT - IF SO ADD TO LOOP
         if(edgeIsUsed == 0 && layerEdges[i].p1.X == (*currentPoint).X && layerEdges[i].p1.Y == (*currentPoint).Y){

            loops[currentLoop][loopI] = layerEdges[i];
            loopI++;

            edgesUsed[numEdgesUsed] = i;
            numEdgesUsed++;

            (*currentPoint).X = layerEdges[i].p2.X;
            (*currentPoint).Y = layerEdges[i].p2.Y;


            edgeAddedThisLoop = 1;

         }


      }


      if(edgeAddedThisLoop == 0){
         
         edgesPerLoop[currentLoop] = loopI;
         loopI = 0;
         currentLoop++;




     
         //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

         // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


         lowestDistance = -1;

         startEdge = 0;

         for(int i = 0; i < layerEdgesI; i++){



            //CHECK IF CURRENTLY CONSIDERED EDGE IS ALREADY USED
            int edgeIsUsed = 0;

            for(int i2 = 0; i2 < numEdgesUsed; i2++){

               if(i == edgesUsed[i2]){

                  printf("No swap to edge: %d - USED\n",edgesUsed[i2]);
                  edgeIsUsed = 1;

               }

            }




            if(edgeIsUsed == 0 && (pointDistance(layerEdges[i].p1, *currentPoint) < lowestDistance || lowestDistance < 0)){

               lowestDistance = pointDistance(layerEdges[i].p1, *currentPoint);

               startEdge = i;

            }

         }


         printf("NEW POINT MOVED\n");         
         (*currentPoint).X = layerEdges[startEdge].p1.X;
         (*currentPoint).Y = layerEdges[startEdge].p1.Y;


      }



   }

   //ONE EXTRA ITERATION TO currentLoop BECAUSE IT WILL MISS ONE IN ABOVE ALGORITHM

         
   edgesPerLoop[currentLoop] = loopI;
   loopI = 0;
   currentLoop++;




   ////////////////////////////////////
   //WRITE PERIMETER EDGES FROM LOOPS//
   ////////////////////////////////////

   writeG1("F", (float[1]){EXTERNAL_PERIMETER_SPEED}, fp);


   for(int i = 0; i < currentLoop; i++){

      for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

         printf("LOOP: %d, EDGE: %d - %f, %f TO %f, %f\n",i,i2,loops[i][i2].p1.X,loops[i][i2].p1.Y,loops[i][i2].p2.X,loops[i][i2].p2.Y);

      }

   }




   return;

}






