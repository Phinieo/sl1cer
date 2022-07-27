#include "libs.h"
#include "readSTL.h"

int main(){

   struct tri* triangles = readSTL("cube.stl");


   printf("X: %f, ",triangles[2].p3.X);
   printf("Y: %f, ",triangles[2].p3.Y);
   printf("Z: %f",triangles[2].p3.Z);
   printf("\n");


   return 0;

}
