

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

   writeG1("F",(float[1]){TRAVEL_SPEED},fp);
   writeG1("Z",(float[1]){(*currentPoint).Z},fp);

   return;

}





void writeLayerPerim(struct edge* loops, int numEdges, int* edgesPerLoop, int numLoops, struct point* currentPoint, float* currentExtrusion, FILE *fp){





   //MOVE EDGES TO ABSOLUTE POINTS ON PRINTBED

   for(int i = 0; i < numLoops; i++){

      for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

         loops[i * numEdges + i2] = centerEdge(loops[i * numEdges + i2]);


      }

   }









   //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

   // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


   float lowestDistance = -1;

   int startEdge = 0;

/*


   for(int i = 0; i < numLoops; i++){

      for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

         if( pointDistance(loops[i * numEdges + i2].p1, *currentPoint) < lowestDistance || lowestDistance < 0){

            lowestDistance = pointDistance(loops[i * numEdges + i2].p1, *currentPoint);

            startEdge = i * numEdges + i2;

            printf("NEAREST POINT - Loop: %d, Edge: %d, Dist: %f\n",i,i2,lowestDistance);

         }

      }

   }











   //GCODE WRITE
   //MOVE TO NEAREST POINT ON THIS LAYER

   (*currentPoint).X = loops[startEdge].p1.X;
   (*currentPoint).Y = loops[startEdge].p1.Y;

   writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

   writeG1("XY", (float[2]){loops[startEdge].p1.X, loops[startEdge].p1.Y}, fp);


*/




   //GCODE WRITE
   //MOVE TO NEAREST POINT ON THIS LAYER

   (*currentPoint).X = loops[0].p1.X;
   (*currentPoint).Y = loops[0].p1.Y;

   writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

   writeG1("XY", (float[2]){loops[0].p1.X, loops[0].p1.Y}, fp);








   for(int i = 0; i < numLoops; i++){

      //SET SPEED TO PERIMETER
      writeG1("F", (float[1]){PERIMETER_SPEED}, fp);

      //WRITE ONE LOOP
      for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

         
         for(int i3 = 0; i3 < edgesPerLoop[i]; i3++){



            if((*currentPoint).X == loops[i * numEdges + i3].p1.X && (*currentPoint).Y == loops[i * numEdges + i3].p1.Y){

               (*currentExtrusion) += perimeterExtrusion(pointDistance(loops[i * numEdges + i3].p1, loops[i * numEdges + i3].p2));

               writeG1("XYE", (float[3]){loops[i * numEdges + i3].p2.X, loops[i * numEdges + i3].p2.Y, (*currentExtrusion)}, fp);

               printf("WROTE LINE!\n");

               (*currentPoint).X = loops[i * numEdges + i3].p2.X;
               (*currentPoint).Y = loops[i * numEdges + i3].p2.Y;

               break;

            }




         }


      }





      //MOVE TO NEXT LOOP

      lowestDistance = -1;

      startEdge = -1;

      for(int i2 = 0; i2 < numLoops; i2++){

         if(i2 > i){

            for(int i3 = 0; i3 < edgesPerLoop[i2]; i3++){

               if(pointDistance(loops[i2 * numEdges + i3].p1, *currentPoint) < lowestDistance || lowestDistance < 0){

                  lowestDistance = pointDistance(loops[i2 * numEdges + i3].p1, *currentPoint);

                  startEdge = i2 * numEdges + i3;

               }

            }

         }

      }


      if(startEdge != -1){


         (*currentPoint).X = loops[startEdge].p1.X;
         (*currentPoint).Y = loops[startEdge].p1.Y;


         writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

         writeG1("XY", (float[2]){(*currentPoint).X, (*currentPoint).Y}, fp);


      }


   }







   printf("FINISHED LAYER\n\n");

   return;

}























/*
void writeLayerPerim(struct edge* loops, int numEdges, int* edgesPerLoop, int numLoops, struct point* currentPoint, float* currentExtrusion, FILE *fp){


   //MOVE EDGES TO ABSOLUTE POINTS ON PRINTBED

   for(int i = 0; i < numLoops; i++){

      for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

         loops[i * numEdges + i2] = centerEdge(layerEdges[i * numEdges + i2]);

      }

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


*/

