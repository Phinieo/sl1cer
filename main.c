#include "libs/libs.h"
#include "configs.h"

#include "libs/structs.h"
#include "libs/readSTL.h"
#include "libs/slicing.h"
#include "libs/extrusion.h"
#include "libs/gcode.h"



int main(){


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
   









/*
   unsigned int numTriangles = 0;

   struct tri* triangles = readSTL(STL_IN, &numTriangles);
*/

   unsigned int numTriangles = 0;

   struct tri* triangles;

   if(binaryOrAsciiSTL(STL_IN)){

      triangles = readBinarySTL(STL_IN, &numTriangles);

   }else{

      triangles = readAsciiSTL(STL_IN, &numTriangles);

   }






   int numVertices = 0;

   struct point* verticeList = calloc(numTriangles*3, sizeof(struct point));


   for(int i = 0; i < numTriangles; i++){


      addUniquePoint(triangles[i].p1, verticeList,&numVertices);

      addUniquePoint(triangles[i].p2, verticeList,&numVertices);

      addUniquePoint(triangles[i].p3, verticeList,&numVertices);



   }

   free(verticeList);

   printf("\n\n\n\n\n\n%d UNIQUE VERTICES\n\n\n\n\n\n",numVertices);





   int numEdges = 0;

   do{

      struct edge* layerEdges = slice(triangles, numTriangles, currentLocation.Z, &numEdges);


      int numLoops = countLoops(layerEdges, numEdges);

      for(int i = 0; i < numLoops; i++){

         int numLoopEdges = 0;

         struct edge* test = getLoop(layerEdges, numEdges, i, &numLoopEdges);

         writeLoop(test, numLoopEdges, &currentLocation, &currentExtrusion, fp);


         for(int i2 = 0; i2 < (PERIMETERS - 1); i2++){

            shrinkLoop(test, INTERNAL_PERIMETER_WIDTH);
            writeLoop(test, numLoopEdges, &currentLocation, &currentExtrusion, fp);


         }



         free(test);

      }


      layerUp(&currentLocation, fp);


      free(layerEdges);

      //free(loops);


   }while(numEdges > 0);








   //WRITE THE END GCODE
   writeEnd(fp);


   //CLOSE THE FILE POINTER
   fclose(fp);



   struct edge testEdge;

   testEdge.p1.X = 1.0;
   testEdge.p1.Y = 0.0;
   testEdge.p1.Z = 0.0;

   testEdge.p2.X = -1.0;
   testEdge.p2.Y = 0.0;
   testEdge.p2.Z = 0.0;

   testEdge.normal.X = 0.0;
   testEdge.normal.Y = 1.0;
   testEdge.normal.Z = 0.0;

   
   printf("\nINPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",testEdge.p1.X,testEdge.p1.Y,testEdge.p2.X,testEdge.p2.Y,testEdge.normal.X,testEdge.normal.Y,testEdge.normal.Z);

   testEdge = scaleEdgeInwards(testEdge, 0.5);

   printf("\nOUTPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",testEdge.p1.X,testEdge.p1.Y,testEdge.p2.X,testEdge.p2.Y,testEdge.normal.X,testEdge.normal.Y,testEdge.normal.Z);

   testEdge = shrinkEdge(testEdge, 0.5);

   printf("\nOUTPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",testEdge.p1.X,testEdge.p1.Y,testEdge.p2.X,testEdge.p2.Y,testEdge.normal.X,testEdge.normal.Y,testEdge.normal.Z);







   return 0;

}
