#include "libs/libs.h"
#include "configs.h"

#include "libs/structs.h"
#include "libs/readSTL.h"
#include "libs/slicing.h"
#include "libs/infill.h"
#include "libs/extrusion.h"
#include "libs/gcode.h"

#include <time.h>

int main(){

   clock_t start, end;
   double cpu_time_used;

   start = clock();


   FILE *fp;

   fp = fopen(GCODE_OUT, "w");


   //WRITE STARTING GCODES INCLUDING HOMING AND TEMPERATURES
   //N
   writeStart(fp);

   fputs("\n\n",fp);



   //EXTRUSION AND POSITION VARIABLES FOR CURRENT STATE OF PRINTHEAD
   struct point currentLocation;
   currentLocation.X = 0;
   currentLocation.Y = 0;
   currentLocation.Z = 0;

   float currentExtrusion = 0;



   //UNDO EARLY EXTRUDER RETRACTION AND Z LIFT

   currentExtrusion += 2;

   writeG1("F",(float[1]){TRAVEL_SPEED},fp);

   writeG1("E",(float[1]){currentExtrusion},fp);

   writeG1("Z",(float[1]){currentLocation.Z},fp);





   //DO PRINTING
   


   //DETECT STL INPUT TYPE AND READ FILE
   //READS INTO LIST OF TRIANGLES
   unsigned int numTriangles = 0;

   struct tri* triangles;

   if(binaryOrAsciiSTL(STL_IN)){

      triangles = readBinarySTL(STL_IN, &numTriangles);

   }else{

      triangles = readAsciiSTL(STL_IN, &numTriangles);

   }




   //CENTER INPUT GEOMETRY TO PRINTBED
   triangles = centerOnPrintBed(triangles, numTriangles);





   //CHECK NUMBER OF TRIANGLES
   int numVertices = 0;

   struct point* verticeList = calloc(numTriangles*3, sizeof(struct point));

   for(int i = 0; i < numTriangles; i++){

      addUniquePoint(triangles[i].p1, verticeList,&numVertices);

      addUniquePoint(triangles[i].p2, verticeList,&numVertices);

      addUniquePoint(triangles[i].p3, verticeList,&numVertices);

   }

   free(verticeList);

   printf("\n\n\n\n\n\n%d UNIQUE VERTICES\n\n\n\n\n\n",numVertices);





   //MAIN PRINTING LOOP
   int numEdges = 0;

   do{

      printf("CURRENT HEIGHT: %f\n\n", currentLocation.Z);

      //GET A LIST OF EDGES WHICH ARE ON THE CURRENT SLICING PLANE
      struct edge* layerEdges = sliceFaster(triangles, numTriangles, currentLocation.Z, &numEdges);


      //COUNT NUMBER OF LOOPS IN THE LIST OF EDGES
      int numLoops = countLoops(layerEdges, numEdges);


      //FOR EACH LOOP - PRINT THE LOOP AND ANY INTERIOR PERIMETERS
      for(int i = 0; i < numLoops; i++){

         int numLoopEdges = 0;

         //GET THE GIVEN LOOP AS A LIST OF CONSECUTIVE EDGES
         struct edge* currentLoop = getLoop(layerEdges, numEdges, i, &numLoopEdges);

         //GENERATE GCODE FOR GIVEN LOOP
         writeLoop(currentLoop, numLoopEdges, &currentLocation, &currentExtrusion, fp);


         //WRITE INTERIOR PERIMETERS
         for(int i2 = 0; i2 < (PERIMETERS - 1); i2++){

            //CREATES A SHRUNKEN LOOP INSIDE A GIVEN LOOP
            struct edge* temp = shrinkLoop(currentLoop, numLoopEdges, INTERNAL_PERIMETER_WIDTH);
            free(currentLoop);
            currentLoop = temp;

            writeLoop(currentLoop, numLoopEdges, &currentLocation, &currentExtrusion, fp);

         }

         //generateInfill(currentLoop, numLoopEdges);

         free(currentLoop);



      }


      layerUp(&currentLocation, fp);


      free(layerEdges);


   }while(numEdges > 0);








   //WRITE THE END GCODE
   writeEnd(fp);


   //CLOSE THE FILE POINTER
   fclose(fp);


   end = clock();

   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

   printf("\n\nTIME USED: %f\n\n",cpu_time_used);


   return 0;

}
