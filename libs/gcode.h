

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










struct point centerPoint(struct point p){

   struct point temp;


   temp.X = p.X + (MAX_X/2);
   temp.Y = p.Y + (MAX_Y/2);
   temp.Z = p.Z;


   return temp;

}




struct tri centerTriangle(struct tri t){

   struct tri temp;

   temp.p1 = centerPoint(t.p1);
   temp.p2 = centerPoint(t.p2);
   temp.p3 = centerPoint(t.p3);

   temp.normal = t.normal;


   return temp;

}




struct edge centerEdge(struct edge e){

   struct edge temp;

   temp.p1 = centerPoint(e.p1);
   temp.p2 = centerPoint(e.p2);

   temp.normal = e.normal;

   return temp;

}





//DETERMINES THE MODEL'S MIN AND MAX X AN Y
//CENTERS THE MODEL INSIDE THE POSITIVE X,Y SPACE ON THE PRINTBED
//MAKES THE MODEL'S LOWEST POINT BE Z = 0

//CHANGES INPUT EDGE LIST
struct tri* centerOnPrintBed(struct tri* triangles, unsigned int numTriangles){


   float maxInputX = FLT_MIN;
   float minInputX = FLT_MAX;

   float maxInputY = FLT_MIN;
   float minInputY = FLT_MAX;

   float minInputZ = FLT_MAX;


   //DETERMINE MIN AND MAX XYZ VALUES
   for(int i = 0; i < numTriangles; i++){

      //CHECK P1
      if(triangles[i].p1.X < minInputX){ minInputX = triangles[i].p1.X; }
      if(triangles[i].p1.X > maxInputX){ maxInputX = triangles[i].p1.X; }

      if(triangles[i].p1.Y < minInputY){ minInputY = triangles[i].p1.Y; }
      if(triangles[i].p1.Y > maxInputY){ maxInputY = triangles[i].p1.Y; }

      if(triangles[i].p1.Z < minInputZ){ minInputZ = triangles[i].p1.Z; }


      //CHECK P2
      if(triangles[i].p2.X < minInputX){ minInputX = triangles[i].p2.X; }
      if(triangles[i].p2.X > maxInputX){ maxInputX = triangles[i].p2.X; }

      if(triangles[i].p2.Y < minInputY){ minInputY = triangles[i].p2.Y; }
      if(triangles[i].p2.Y > maxInputY){ maxInputY = triangles[i].p2.Y; }

      if(triangles[i].p2.Z < minInputZ){ minInputZ = triangles[i].p2.Z; }


      //CHECK P3
      if(triangles[i].p3.X < minInputX){ minInputX = triangles[i].p3.X; }
      if(triangles[i].p3.X > maxInputX){ maxInputX = triangles[i].p3.X; }

      if(triangles[i].p3.Y < minInputY){ minInputY = triangles[i].p3.Y; }
      if(triangles[i].p3.Y > maxInputY){ maxInputY = triangles[i].p3.Y; }

      if(triangles[i].p3.Z < minInputZ){ minInputZ = triangles[i].p3.Z; }
      

   }


   float xCenter = maxInputX - ((maxInputX - minInputX)/2);
   float yCenter = maxInputY - ((maxInputY - minInputY)/2);

   printf("\n\n\n X CENTER: %f\nY CENTER: %f\n\n\n",xCenter,yCenter);

   
   //APPLY TRANSFORMATION TO GET ALL POINTS CENTERED AROUND Z AXIS AND LIEING ABOVE XY PLANE
   
   //AFTER CENTERING AROUND AXIS, CENTER TO BUILD PLATE
   for(int i = 0; i < numTriangles; i++){

      //MOVE P1 TO ORIGIN
      triangles[i].p1.X = triangles[i].p1.X - xCenter;
      triangles[i].p1.Y = triangles[i].p1.Y - yCenter;

      triangles[i].p1.Z = triangles[i].p1.Z - minInputZ;

      //MOVE P2 TO ORIGIN
      triangles[i].p2.X = triangles[i].p2.X - xCenter;
      triangles[i].p2.Y = triangles[i].p2.Y - yCenter;

      triangles[i].p2.Z = triangles[i].p2.Z - minInputZ;

      //MOVE P3 TO ORIGIN
      triangles[i].p3.X = triangles[i].p3.X - xCenter;
      triangles[i].p3.Y = triangles[i].p3.Y - yCenter;

      triangles[i].p3.Z = triangles[i].p3.Z - minInputZ;


      //CENTER EDGE TO BUILD PLATE
      triangles[i] = centerTriangle(triangles[i]);

      printf("CENTERED TRIANGLE:\n");
      printf("P1: %f, %f, %f\n",triangles[i].p1.X,triangles[i].p1.Y,triangles[i].p1.Z);
      printf("P2: %f, %f, %f\n",triangles[i].p2.X,triangles[i].p2.Y,triangles[i].p2.Z);
      printf("P3: %f, %f, %f\n",triangles[i].p3.X,triangles[i].p3.Y,triangles[i].p3.Z);

   }


   return triangles;

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








//POSSIBLE PROBLEM: MOVES TO FIRST POINT, NOT CLOSEST POINT
void writeLoop(struct edge* loopIN, int numEdges, struct point* currentPoint, float* currentExtrusion, FILE *fp){


   //MAKE NEW ARRAY SO AS TO NOT CHANGE INPUT LOOP ARRAY
   struct edge* loop = (struct edge*)calloc(sizeof(struct edge),numEdges);

   //MAY NOT BE NECESSARY ANYMORE
   for(int i = 0; i < numEdges; i++){

      //loop[i] = centerEdge(loopIN[i]);
      loop[i] = loopIN[i];

   }



   //GCODE WRITE
   //MOVE TO FIRST POINT ON THIS LAYER

   (*currentPoint).X = loop[0].p1.X;
   (*currentPoint).Y = loop[0].p1.Y;

   writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

   writeG1("XY", (float[2]){loop[0].p1.X, loop[0].p1.Y}, fp);



   //END RETRACTION BEFORE PRINTING LOOPS
   endRetract(currentExtrusion, fp);


   //SET SPEED FOR PERIMETER PRINTING
   writeG1("F", (float[1]){PERIMETER_SPEED}, fp);

   //PRINT PERIMETER
   for(int i = 0; i < numEdges; i++){

      if((*currentPoint).X == loop[i].p1.X && (*currentPoint).Y == loop[i].p1.Y){

         (*currentExtrusion) += perimeterExtrusion(pointDistance(loop[i].p1, loop[i].p2));

         writeG1("XYE", (float[3]){loop[i].p2.X, loop[i].p2.Y, (*currentExtrusion)}, fp);

         (*currentPoint).X = loop[i].p2.X;
         (*currentPoint).Y = loop[i].p2.Y;

      }

   }


   //START RETRACTION FOR NEXT LOOP OR MOVEMENT
   startRetract(currentExtrusion, fp);


   free(loop);

   return;

}




//WRITES A LIST OF EDGES IN THE PROVIDED ORDER
//USED FOR PRINTING INFILL
void writeEdges(struct edge* edges, int numEdges, struct point* currentPoint, float* currentExtrusion, FILE *fp){





   for(int i = 0; i < numEdges; i++){

      //MOVE TO EDGE START
      (*currentPoint).X = edges[0].p1.X;
      (*currentPoint).Y = edges[0].p1.Y;

      writeG1("F", (float[1]){TRAVEL_SPEED}, fp);

      writeG1("XY", (float[2]){edges[i].p1.X, edges[i].p1.Y}, fp);



      //END RETRACT TO START PRINTING
      endRetract(currentExtrusion, fp);


      //SET SPEED FOR PRINTING INFILL
      writeG1("F", (float[1]){INFILL_SPEED}, fp);

      (*currentExtrusion) += infillExtrusion(pointDistance(edges[i].p1, edges[i].p2));

      writeG1("XYE", (float[3]){edges[i].p2.X, edges[i].p2.Y, (*currentExtrusion)}, fp);




      //START RETRACT FOR NEXT TRAVEL
      startRetract(currentExtrusion, fp);


   }


   return;

}




