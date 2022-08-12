#include "libs.h"
#include "configs.h"
#include "readSTL.h"
#include "gcode.h"
#include "slicing.h"



int main(){


   FILE *fp;

   fp = fopen(GCODE_OUT, "w");


   //WRITE STARTING GCODES INCLUDING HOMING AND TEMPERATURES
   //N
   writeStart(fp);

   fputs("\n\n",fp);





   //UNDO EARLY EXTRUDER RETRACTION

   fputs("G1 E2.00000 F2400.00000",fp);

   fputs("\n\n",fp);






   //DO PRINTING
   





   int numTriangles = 0;

   struct tri* triangles = readSTL(STL_IN, &numTriangles);

   for(int i = 0; i < numTriangles; i++){

      printf("Triangle %d\n",i);

      printf("X: %f, ",triangles[i].p1.X);
      printf("Y: %f, ",triangles[i].p1.Y);
      printf("Z: %f\n",triangles[i].p1.Z);

      printf("X: %f, ",triangles[i].p2.X);
      printf("Y: %f, ",triangles[i].p2.Y);
      printf("Z: %f\n",triangles[i].p2.Z);

      printf("X: %f, ",triangles[i].p3.X);
      printf("Y: %f, ",triangles[i].p3.Y);
      printf("Z: %f\n",triangles[i].p3.Z);

      printf("Normal: ");
      printf("X: %f, ",triangles[i].normal.X);
      printf("Y: %f, ",triangles[i].normal.Y);
      printf("Z: %f\n",triangles[i].normal.Z);



      printf("\n\n");
     

   }

   int numEdges = 0;

   slice(triangles, numTriangles, 40.2, &numEdges);

   printf("\nMAIN: NUM EDGES: %d\n",numEdges);







   //WRITE THE END GCODE
   writeEnd(fp);


   //CLOSE THE FILE POINTER
   fclose(fp);




   return 0;

}
