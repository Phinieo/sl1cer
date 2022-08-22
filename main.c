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



   //UNDO EARLY EXTRUDER RETRACTION

   fputs("G1 E2.00000 F2400.00000",fp);

   fputs("\n\n",fp);

   currentExtrusion += 2;





   //DO PRINTING
   










   int numTriangles = 0;

   struct tri* triangles = readSTL(STL_IN, &numTriangles);




   int numEdges = 0;

   do{

      struct edge* layerEdges = slice(triangles, numTriangles, currentLocation.Z, &numEdges);


      int* edgesPerLoop;

      int currentLoop = 0;


      struct edge* loops = edgesToLoops(layerEdges, numEdges, currentLocation, edgesPerLoop, &currentLoop);

      printf("\nCURRENTLOOP: %d\n",currentLoop);

      printf("\n%f, %f\n",loops[1].p2.X,loops[1].p2.Y);
//      printf("MAIN: last loops val: %f, %f",loops[((currentLoop-1) * numEdges/3) + edgesPerLoop[currentLoop-1]-1].p2.X,loops[((currentLoop-1) * numEdges/3) + edgesPerLoop[currentLoop-1]-1].p2.Y);

      //writeLayerPerim(layerEdges, numEdges, &currentLocation, &currentExtrusion, fp);


      layerUp(&currentLocation, fp);


      free(layerEdges);



   }while(numEdges > 0);








   //WRITE THE END GCODE
   writeEnd(fp);


   //CLOSE THE FILE POINTER
   fclose(fp);




   return 0;

}
