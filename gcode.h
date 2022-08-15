

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


   writeG1("XY", (float[2]){layerEdges[startEdge].p1.X, layerEdges[startEdge].p1.Y}, fp);






   //

   int edgesUsed = 0;

   while(edgesUsed < *layerEdgesI){

      for(int i = 0; i < *layerEdgesI; i++){

         if((edgesUsed < *layerEdgesI) && layerEdges[i].p1.X == (*currentPoint).X && layerEdges[i].p1.Y == (*currentPoint).Y){


            (*currentPoint).X = layerEdges[i].p2.X;
            (*currentPoint).Y = layerEdges[i].p2.Y;

            writeG1("XY", (float[2]){layerEdges[i].p2.X, layerEdges[i].p2.Y}, fp);


            edgesUsed++;



         }

      }

   }






   return;

}






