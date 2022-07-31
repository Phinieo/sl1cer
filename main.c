#include "libs.h"
#include "readSTL.h"
#include "configs.h"
#include "slicing.h"



int main(){


   int numTriangles = 0;

   struct tri* triangles = readSTL("cube.stl", &numTriangles);

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

      printf("\n\n");
     

   }


   slice(triangles, numTriangles);


/*
   printf("X: %f, ",triangles[2].p3.X);
   printf("Y: %f, ",triangles[2].p3.Y);
   printf("Z: %f",triangles[2].p3.Z);
   printf("\n");
*/

   return 0;

}
