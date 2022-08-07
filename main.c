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





   printf("\n\n\nINTERSECTION TEST\n\n");


   struct tri testTri;
   testTri.p1.X = 5;
   testTri.p1.Y = 5;
   testTri.p1.Z = 1;

   testTri.p2.X = -5;
   testTri.p2.Y = -5;
   testTri.p2.Z = 0;

   testTri.p3.X = 0;
   testTri.p3.Y = 0;
   testTri.p3.Z = 10;


   struct point tempPoint = intersectLine(0.1, testTri.p1, testTri.p2);


   printf("%d\n\n",pointIsOnTri(tempPoint,testTri));


   return 0;

}
