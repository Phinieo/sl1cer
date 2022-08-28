

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




void startRetract(float* currentExtrusion, FILE *fp){

   (*currentExtrusion) -= RETRACTION_DISTANCE;

   writeG1("F",(float[1]){TRAVEL_SPEED},fp);
   writeG1("E",(float[1]){(*currentExtrusion)},fp);

   return;

}


void endRetract(float* currentExtrusion, FILE *fp){

   (*currentExtrusion) += RETRACTION_DISTANCE;

   writeG1("F",(float[1]){TRAVEL_SPEED},fp);
   writeG1("E",(float[1]){(*currentExtrusion)},fp);

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



   endRetract(currentExtrusion, fp);




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

         startRetract(currentExtrusion, fp);


         (*currentPoint).X = loops[startEdge].p1.X;
         (*currentPoint).Y = loops[startEdge].p1.Y;


         writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

         writeG1("XY", (float[2]){(*currentPoint).X, (*currentPoint).Y}, fp);

         endRetract(currentExtrusion, fp);


      }


   }




   startRetract(currentExtrusion, fp);


   printf("FINISHED LAYER\n\n");

   return;

}




