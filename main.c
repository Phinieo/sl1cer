#include "libs.h"
#include "configs.h"

#include "structs.h"
#include "readSTL.h"
#include "slicing.h"
#include "extrusion.h"
#include "gcode.h"



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
   










   int numTriangles = 0;

   struct tri* triangles = readSTL(STL_IN, &numTriangles);




   int numEdges = 0;

   do{

      struct edge* layerEdges = slice(triangles, numTriangles, currentLocation.Z, &numEdges);


      int* edgesPerLoop;

      int currentLoop = 0;


      struct edge* loops = edgesToLoops(layerEdges, numEdges, currentLocation, &edgesPerLoop, &currentLoop);


      writeLayerPerim(loops, numEdges, edgesPerLoop, currentLoop, &currentLocation, &currentExtrusion, fp);





      for(int i = 0; i < PERIMETERS - 1; i++){

         printf("INTERIOR PERIMETER; %d\n",i);

         struct edge* temp = shrinkLoops(loops, numEdges, edgesPerLoop, currentLoop);

         free(loops);

         loops = temp;


         writeLayerPerim(loops, numEdges, edgesPerLoop, currentLoop, &currentLocation, &currentExtrusion, fp);



      }


      layerUp(&currentLocation, fp);


      free(layerEdges);

      free(loops);


   }while(numEdges > 0);








   //WRITE THE END GCODE
   writeEnd(fp);


   //CLOSE THE FILE POINTER
   fclose(fp);




   return 0;

}
