
struct edge{

   struct point p1;
   struct point p2;

};





//p2 MUST BE HIGHER Z THAN p1
//p1 AND p2 MUST BE INTERSECTED BY sliceHeight
struct point intersectLine(float sliceHeight, struct point p1, struct point p2){

   float dX = p2.X - p1.X;
   float dY = p2.Y - p1.Y;
   float dZ = p2.Z - p1.Z;

   float percent = (sliceHeight - p1.Z) / dZ;

   float X = (dX * percent) + p1.X;
   float Y = (dY * percent) + p1.Y;
   float Z = sliceHeight;

   struct point intersectionPoint;
   intersectionPoint.X = X;
   intersectionPoint.Y = Y;
   intersectionPoint.Z = Z;

   return intersectionPoint;

}


//pointIsOnTri FUNCTION BUT ONLY FOR THE LAST CONDITIONAL LAYER
int pointIsAboveTri(struct point p, struct tri triangle){


   //IF POINT IS AN EXACT POINT OF THE TRIANGLE EXCEPT FOR Z

   if(triangle.p1.X == p.X && triangle.p1.Y == p.Y && triangle.p1.Z < p.Z){

      return 1;

   }


   if(triangle.p2.X == p.X && triangle.p2.Y == p.Y && triangle.p2.Z < p.Z){

      return 1;

   }


   if(triangle.p3.X == p.X && triangle.p3.Y == p.Y && triangle.p3.Z < p.Z){

      return 1;

   }




   return 0;

}



int pointIsOnTri(struct point p, struct tri triangle){


   //IF POINT IS AN EXACT POINT OF THE TRIANGLE

   if(triangle.p1.X == p.X && triangle.p1.Y == p.Y && triangle.p1.Z == p.Z){

      return 1;

   }


   if(triangle.p2.X == p.X && triangle.p2.Y == p.Y && triangle.p2.Z == p.Z){

      return 1;

   }


   if(triangle.p3.X == p.X && triangle.p3.Y == p.Y && triangle.p3.Z == p.Z){

      return 1;

   }





   //IF POINT IS ON AN EDGE OF THE TRIANGLE

   if((triangle.p2.Z > p.Z && triangle.p1.Z < p.Z) || (triangle.p2.Z < p.Z && triangle.p1.Z > p.Z)){

      if(triangle.p2.Z > triangle.p1.Z){

         struct point temp = intersectLine(p.Z, triangle.p1, triangle.p2);

         if(temp.X == p.X && temp.Y == p.Y){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p2, triangle.p1);

         if(temp.X == p.X && temp.Y == p.Y){

           return 1; 

         }

      }

   }






   if((triangle.p3.Z > p.Z && triangle.p2.Z < p.Z) || (triangle.p3.Z < p.Z && triangle.p2.Z > p.Z)){

      if(triangle.p3.Z > triangle.p2.Z){

         struct point temp = intersectLine(p.Z, triangle.p2, triangle.p3);

         if(temp.X == p.X && temp.Y == p.Y){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p3, triangle.p2);

         if(temp.X == p.X && temp.Y == p.Y){

           return 1; 

         }

      }

   }



   if((triangle.p1.Z > p.Z && triangle.p3.Z < p.Z) || (triangle.p1.Z < p.Z && triangle.p3.Z > p.Z)){

      if(triangle.p1.Z > triangle.p3.Z){

         struct point temp = intersectLine(p.Z, triangle.p3, triangle.p1);

         if(temp.X == p.X && temp.Y == p.Y){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p1, triangle.p3);

         if(temp.X == p.X && temp.Y == p.Y){

           return 1; 

         }

      }

   }








   return 0;

}




//ADDS POINT TO LIST IF IT IS NOT CURRENTLY IN LIST
int addUniquePoint(struct point p, struct point* points, int* numPoints){

   for(int i = 0; i < *numPoints; i++){

      if(p.X == points[i].X && p.Y == points[i].Y && p.Z == points[i].Z){

         return 0;

      }

   }


   points[*numPoints] = p;

   *numPoints = *numPoints + 1;


   return 1;

}










//RETURNED POINTER MUST BE FREE'D
struct edge* pointsToEdges(struct point* layerPoints, int layerPointsI, struct tri* triangles, int numTriangles, int* layerEdgesI){




   //FIND MAX HEIGHT
   float maxHeight = 0;


   for(int i = 0; i < numTriangles; i++){


      if(triangles[i].p1.Z > maxHeight){

         maxHeight = triangles[i].p1.Z;

      }


      if(triangles[i].p2.Z > maxHeight){

         maxHeight = triangles[i].p2.Z;

      }


      if(triangles[i].p3.Z > maxHeight){

         maxHeight = triangles[i].p3.Z;

      }


   }


   struct edge* layerEdges = calloc(layerPointsI*2, sizeof(struct edge));


   *layerEdgesI = 0;

   for(int i = 0; i < layerPointsI; i++){

      for(int i2 = 0; i2 < numTriangles; i2++){

         if(pointIsOnTri(layerPoints[i], triangles[i2]) || (layerPoints[i].Z > maxHeight && pointIsAboveTri(layerPoints[i], triangles[i2]))){

            for(int i3 = 0; i3 < layerPointsI; i3++){

               if(i3 != i && (pointIsOnTri(layerPoints[i3], triangles[i2]) || (layerPoints[i3].Z > maxHeight &&
                   pointIsAboveTri(layerPoints[i3], triangles[i2])))){

                  //CHECK TO SEE IF FIRST EDGE POINT IS ALREADY A FIRST EDGE POINT TO AVOID DUPLICATE LOOP
                     
                  int isInEdgeList = 0;
                  for(int i4 = 0; i4 < *layerEdgesI; i4++){

                     //DO NOT ADD IF ANY PREVIOUS EDGES HAVE THE SAME STARTING POINT AS CURRENT
                     if(layerEdges[i4].p1.X == layerPoints[i].X && layerEdges[i4].p1.Y == layerPoints[i].Y){


                        isInEdgeList = 1;

                     
                     }

                     //DO NOT ADD IF THE CURRENT EDGE IS REVERSE OF ANY PREVIOUS EDGES

                     if(layerEdges[i4].p2.X == layerPoints[i].X && layerEdges[i4].p2.Y == layerPoints[i].Y){


                        if(layerEdges[i4].p1.X == layerPoints[i3].X && layerEdges[i4].p1.Y == layerPoints[i3].Y){

                           isInEdgeList = 1; 

                        }

                     }




                  }
                     
                  //ADD EDGE IF ALL CHECKS ARE GOOD
                  if(!isInEdgeList){

                     layerEdges[*layerEdgesI].p1 = layerPoints[i];
                     layerEdges[*layerEdgesI].p2 = layerPoints[i3];
                     printf("\n\nEDGE: %f,%f and %f,%f",layerEdges[*layerEdgesI].p1.X,layerEdges[*layerEdgesI].p1.Y,layerEdges[*layerEdgesI].p2.X,layerEdges[*layerEdgesI].p2.Y);
                     
                     *layerEdgesI = *layerEdgesI + 1;
                  
                  }

               }

            }

         }

      }

   }


   printf("\n\nEDGES: %d\n\n",*layerEdgesI);











   return layerEdges;

}










int slice(struct tri* triangles, int numTriangles){


   float currentHeight = 0;






   //FIND MAX HEIGHT
   float maxHeight = 0;


   for(int i = 0; i < numTriangles; i++){


      if(triangles[i].p1.Z > maxHeight){

         maxHeight = triangles[i].p1.Z;

      }


      if(triangles[i].p2.Z > maxHeight){

         maxHeight = triangles[i].p2.Z;

      }


      if(triangles[i].p3.Z > maxHeight){

         maxHeight = triangles[i].p3.Z;

      }


   }


   printf("\n\n\n\n\nMAX HEIGHT: %f\n\n\n\n\n\n",maxHeight);









   //SLICE FROM THE BOTTOM UP
   for(currentHeight = 0; currentHeight < maxHeight; currentHeight += LAYER_HEIGHT){















      //not sure what to set the maximum number of possible layer vertices as
      struct point layerPoints[numTriangles];
      int layerPointsI = 0;







      //FIND ALL POINTS THAT LIE ON CURRENT LAYER
      for(int i = 0; i < numTriangles; i++){


         if(triangles[i].p1.Z == currentHeight){

            
            addUniquePoint(triangles[i].p1, layerPoints, &layerPointsI);


         }


         if(triangles[i].p2.Z == currentHeight){
            

            addUniquePoint(triangles[i].p2, layerPoints, &layerPointsI);


         }


         if(triangles[i].p3.Z == currentHeight){

            
            addUniquePoint(triangles[i].p3, layerPoints, &layerPointsI);

         }


      }










      //FIND ALL POINTS THAT LIE ON LINES INTERSECTED BY CURRENT LAYER
      for(int i = 0; i < numTriangles; i++){




         //FIND A POINT BELOW THE CURRENT LAYER
         if(triangles[i].p1.Z < currentHeight){


            if(triangles[i].p2.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p1, triangles[i].p2), layerPoints, &layerPointsI);


            }

            if(triangles[i].p3.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p1, triangles[i].p3), layerPoints, &layerPointsI);


            }


         }






         //FIND A POINT BELOW THE CURRENT LAYER
         if(triangles[i].p2.Z < currentHeight){


            if(triangles[i].p1.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p2, triangles[i].p1), layerPoints, &layerPointsI);


            }

            if(triangles[i].p3.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p2, triangles[i].p3), layerPoints, &layerPointsI);


            }


         }





         //FIND A POINT BELOW THE CURRENT LAYER
         if(triangles[i].p3.Z < currentHeight){


            if(triangles[i].p1.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p3, triangles[i].p1), layerPoints, &layerPointsI);


            }

            if(triangles[i].p2.Z > currentHeight){


               addUniquePoint(intersectLine(currentHeight, triangles[i].p3, triangles[i].p2), layerPoints, &layerPointsI);


            }


         }







      }






      //SLICING DONE (EXCEPT FOR POSSIBLE TOP LAYER) EXECUTE CONVERSION TO G-CODE 



      printf("%d Points\n",layerPointsI);


      for(int i = 0; i < layerPointsI; i++){

         printf("X: %f, Y: %f, Z: %f\n",layerPoints[i].X,layerPoints[i].Y,layerPoints[i].Z);

         for(int i2 = 0; i2 < layerPointsI; i2++){

            if(layerPoints[i].X == layerPoints[i2].X && layerPoints[i].Y == layerPoints[i2].Y && layerPoints[i].Z == layerPoints[i2].Z && i2 != i){

               printf("Duplicate points!\n");

            }

         }

      }







      //CONVERT POINTS TO EDGES

      int layerEdgesI = 0;

      struct edge* layerEdges = pointsToEdges(layerPoints, layerPointsI, triangles, numTriangles, &layerEdgesI);




      //CONVERT EDGES TO GCODE






















      //(THIS IS THE POSSIBLE TOP LAYER) - IF AN EXTRA LAYER IS CLOSER TO REAL OBJECT HEIGHT THAN CURRENT LAYER HEIGHT


      if(currentHeight + LAYER_HEIGHT > maxHeight  && maxHeight - currentHeight > (currentHeight + LAYER_HEIGHT) - maxHeight){

         printf("\n\nFINAL CONDITIONAL LAYER\n\n");


         //not sure what to set the maximum number of possible layer vertices as
         struct point lastPoints[numTriangles];
         layerPointsI = 0;





         //FIND ALL POINTS THAT LIE ON CURRENT LAYER
         for(int i = 0; i < numTriangles; i++){


            if(triangles[i].p1.Z == maxHeight){

            
               struct point temp1 = triangles[i].p1;
               temp1.Z = currentHeight + LAYER_HEIGHT;
               addUniquePoint(temp1, lastPoints, &layerPointsI);


            }


            if(triangles[i].p2.Z == maxHeight){
            
               struct point temp2 = triangles[i].p2;
               temp2.Z = currentHeight + LAYER_HEIGHT;
               addUniquePoint(temp2, lastPoints, &layerPointsI);


            }


            if(triangles[i].p3.Z == maxHeight){

               struct point temp3 = triangles[i].p3;
               temp3.Z = currentHeight + LAYER_HEIGHT;
               addUniquePoint(temp3, lastPoints, &layerPointsI);

            }


         } 






         printf("%d Points\n",layerPointsI);


         for(int i = 0; i < layerPointsI; i++){

            printf("X: %f, Y: %f, Z: %f\n",lastPoints[i].X,lastPoints[i].Y,lastPoints[i].Z);

            for(int i2 = 0; i2 < layerPointsI; i2++){

               if(layerPoints[i].X == lastPoints[i2].X && lastPoints[i].Y == lastPoints[i2].Y && lastPoints[i].Z == lastPoints[i2].Z && i2 != i){

                  printf("Duplicate points!\n");

               }

            }

         }







         layerEdgesI = 0;

         struct edge* layerEdges = pointsToEdges(lastPoints, layerPointsI, triangles, numTriangles, &layerEdgesI);








         //CONVERT CONDITIONAL LAYER TO GCODE







      //END OF CONDITIONAL FINAL LAYER IF
      }

















   //END OF HEIGHT BASED SLICING LOOP
   }




   return 0;

}
