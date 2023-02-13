


//DOES NOT COVER EDGE CASE IN WHICH p1 == p2
//p1 AND p2 MUST BE INTERSECTED BY sliceHeight
struct point intersectLine(float sliceHeight, struct point p1, struct point p2){


   struct point intersectionPoint;

   if(p2.Z > p1.Z){

      float dX = p2.X - p1.X;
      float dY = p2.Y - p1.Y;
      float dZ = p2.Z - p1.Z;

      float percent = (sliceHeight - p1.Z) / dZ;

      intersectionPoint.X = (dX * percent) + p1.X;
      intersectionPoint.Y = (dY * percent) + p1.Y;
      intersectionPoint.Z = sliceHeight;

   }else{

      float dX = p1.X - p2.X;
      float dY = p1.Y - p2.Y;
      float dZ = p1.Z - p2.Z;

      float percent = (sliceHeight - p2.Z) / dZ;

      intersectionPoint.X = (dX * percent) + p2.X;
      intersectionPoint.Y = (dY * percent) + p2.Y;
      intersectionPoint.Z = sliceHeight;
      
   }


   return intersectionPoint;

}






//pointIsOnTri FUNCTION BUT ONLY FOR THE LAST CONDITIONAL LAYER
int pointIsAboveTri(struct point p, struct tri triangle){


   //IF POINT IS AN EXACT POINT OF THE TRIANGLE EXCEPT FOR Z

   if(floatIsEqual(triangle.p1.X, p.X) && floatIsEqual(triangle.p1.Y, p.Y) && triangle.p1.Z < p.Z){

      return 1;

   }


   if(floatIsEqual(triangle.p2.X, p.X) && floatIsEqual(triangle.p2.Y, p.Y) && triangle.p2.Z < p.Z){

      return 1;

   }


   if(floatIsEqual(triangle.p3.X, p.X) && floatIsEqual(triangle.p3.Y, p.Y) && triangle.p3.Z < p.Z){

      return 1;

   }




   return 0;

}









int pointIsOnTri(struct point p, struct tri triangle){


   //IF POINT IS AN EXACT POINT OF THE TRIANGLE

   if(floatIsEqual(triangle.p1.X, p.X) && floatIsEqual(triangle.p1.Y, p.Y) && floatIsEqual(triangle.p1.Z, p.Z)){

      return 1;

   }


   if(floatIsEqual(triangle.p2.X, p.X) && floatIsEqual(triangle.p2.Y, p.Y) && floatIsEqual(triangle.p2.Z, p.Z)){

      return 1;

   }


   if(floatIsEqual(triangle.p3.X, p.X) && floatIsEqual(triangle.p3.Y, p.Y) && floatIsEqual(triangle.p3.Z, p.Z)){

      return 1;

   }





   //IF POINT IS ON AN EDGE OF THE TRIANGLE

   if((triangle.p2.Z > p.Z && triangle.p1.Z < p.Z) || (triangle.p2.Z < p.Z && triangle.p1.Z > p.Z)){

      if(triangle.p2.Z > triangle.p1.Z){

         struct point temp = intersectLine(p.Z, triangle.p1, triangle.p2);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p2, triangle.p1);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

           return 1; 

         }

      }

   }






   if((triangle.p3.Z > p.Z && triangle.p2.Z < p.Z) || (triangle.p3.Z < p.Z && triangle.p2.Z > p.Z)){

      if(triangle.p3.Z > triangle.p2.Z){

         struct point temp = intersectLine(p.Z, triangle.p2, triangle.p3);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p3, triangle.p2);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

           return 1; 

         }

      }

   }



   if((triangle.p1.Z > p.Z && triangle.p3.Z < p.Z) || (triangle.p1.Z < p.Z && triangle.p3.Z > p.Z)){

      if(triangle.p1.Z > triangle.p3.Z){

         struct point temp = intersectLine(p.Z, triangle.p3, triangle.p1);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

            return 1; 

         }

      }else{


         struct point temp = intersectLine(p.Z, triangle.p1, triangle.p3);

         if(floatIsEqual(temp.X, p.X) && floatIsEqual(temp.Y, p.Y)){

           return 1; 

         }

      }

   }



   return 0;

}




//ADDS POINT TO LIST IF IT IS NOT CURRENTLY IN LIST
int addUniquePoint(struct point p, struct point* points, int* numPoints){

   for(int i = 0; i < *numPoints; i++){

      if(floatIsEqual(p.X, points[i].X) && floatIsEqual(p.Y, points[i].Y) && floatIsEqual(p.Z, points[i].Z)){

         return 0;

      }

   }


   points[*numPoints] = p;

   *numPoints = *numPoints + 1;


   return 1;

}










//ADDS EDGE TO LIST IF IT IS NOT CURRENTLY IN LIST
int addUniqueEdge(struct edge e, struct edge* edges, int* numEdges){


   for(int i = 0; i < *numEdges; i++){

      //CHECK FOR EXACT EDGE MATCH
      if(e.p1.X == edges[i].p1.X && e.p1.Y == edges[i].p1.Y){

         if(e.p2.X == edges[i].p2.X && e.p2.Y == edges[i].p2.Y){


            return 0;


         }


      }



      //CHECK FOR REVERSE EDGE MATCH
      if(e.p1.X == edges[i].p2.X && e.p1.Y == edges[i].p2.Y){

         if(e.p2.X == edges[i].p1.X && e.p2.Y == edges[i].p1.Y){

            return 0;


         }


      }



   }


   edges[*numEdges] = e;

   *numEdges = *numEdges + 1;


   return 1;



}





int countLoops(struct edge* edges, int numEdges){

   int numLoops = 0;


   struct point currentPoint;

   currentPoint.X = edges[0].p1.X;
   currentPoint.Y = edges[0].p1.Y;
   currentPoint.Z = edges[0].p1.Z;


   //STRUCTURE TO SAVE THE COUNTED EDGES IN
   struct edge* usedEdges = calloc(numEdges, sizeof(struct edge));
   int numUsedEdges = 0;


   int loopOngoingFlag = 0;

   while(numUsedEdges < numEdges){

      loopOngoingFlag = 0;

      for(int i = 0; i < numEdges; i++){

         if(currentPoint.X == edges[i].p1.X && currentPoint.Y == edges[i].p1.Y, currentPoint.Z == edges[i].p1.Z){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               printf("\n\n\nDEBUG1!!!!!!\n\n\n\n");

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               loopOngoingFlag += 1;

            }

         }else if(currentPoint.X == edges[i].p2.X && currentPoint.Y == edges[i].p2.Y, currentPoint.Z == edges[i].p2.Z){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               printf("\n\n\nDEBUG1!!!!!!\n\n\n\n");

               currentPoint.X = edges[i].p1.X;
               currentPoint.Y = edges[i].p1.Y;
               currentPoint.Z = edges[i].p1.Z;

               loopOngoingFlag += 1;

            }

         }


      }


      if(loopOngoingFlag == 0){

         numLoops++;

         printf("\n\n\nDEBUG2!!!!!!\n\n\n\n");

         for(int i = 0; i < numEdges; i++){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               break;

            }

         }



      }

      printf("\n\n\nDEBUG3!!!!!!\n\n\n\n");

   }

   free(usedEdges);

   numLoops++;

   return numLoops;

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



   //COMPARE EACH POINT TO EACH OTHER POINT
   for(int p1 = 0; p1 < layerPointsI; p1++){

      for(int p2 = 0; p2 < layerPointsI; p2++){


         //DO NOT COMPARE A POINT TO ITSELF
         if(p1 != p2){

            //CHECK POINTS TO EACH TRIANGLE
            for(int t = 0; t < numTriangles; t++){


               //SKIP UP OR DOWN FACING TRIANGLES
               if(triFacesUp(triangles[t]) || triFacesDown(triangles[t])){

                  continue;

               }

               //IF POINTS LIE ON THE SAME TRIANGLE ADD THEM TO THE LIST OF UNIQUE EDGES
               if(pointIsOnTri(layerPoints[p1],triangles[t]) && pointIsOnTri(layerPoints[p2],triangles[t])){

                  struct edge tempEdge;
                  tempEdge.p1.X = layerPoints[p1].X;
                  tempEdge.p1.Y = layerPoints[p1].Y;
                  tempEdge.p1.Z = layerPoints[p1].Z;

                  tempEdge.p2.X = layerPoints[p2].X;
                  tempEdge.p2.Y = layerPoints[p2].Y;
                  tempEdge.p2.Z = layerPoints[p2].Z;

                  tempEdge.normal = normal3Dto2D(triangles[t].normal);

                  //PREVENTS IDENTICAL OR REVERSED EDGES
                  if(addUniqueEdge(tempEdge,layerEdges,layerEdgesI)){

                     printf("\n\nEDGE: %f,%f and %f,%f",layerEdges[*layerEdgesI-1].p1.X,layerEdges[*layerEdgesI-1].p1.Y,layerEdges[*layerEdgesI-1].p2.X,layerEdges[*layerEdgesI-1].p2.Y);
 
                  }

               }

               //IF ABOVE MAX HEIGHT AND POINTS LIE ABOVE THE SAME TRIANGLE ADD THEM TO THE LIST OF UNIQUE EDGES
               if(layerPoints[p1].Z > maxHeight && (pointIsAboveTri(layerPoints[p1],triangles[t]) && pointIsAboveTri(layerPoints[p2],triangles[t]))){

                  struct edge tempEdge;
                  tempEdge.p1.X = layerPoints[p1].X;
                  tempEdge.p1.Y = layerPoints[p1].Y;
                  tempEdge.p1.Z = layerPoints[p1].Z;

                  tempEdge.p2.X = layerPoints[p2].X;
                  tempEdge.p2.Y = layerPoints[p2].Y;
                  tempEdge.p2.Z = layerPoints[p2].Z;

                  tempEdge.normal = normal3Dto2D(triangles[t].normal);

                  //PREVENTS IDENTICAL OR REVERSED EDGES
                  if(addUniqueEdge(tempEdge,layerEdges,layerEdgesI)){

                     printf("\n\nEDGE: %f,%f and %f,%f",layerEdges[*layerEdgesI-1].p1.X,layerEdges[*layerEdgesI-1].p1.Y,layerEdges[*layerEdgesI-1].p2.X,layerEdges[*layerEdgesI-1].p2.Y);
 
                  }

               }

            

            }



         }



      }

   }



   printf("\n\nEDGES: %d\n\n",*layerEdgesI);











   return layerEdges;

}






struct edge scaleEdgeInwards(struct edge bigEdge){

   struct edge smallEdge = bigEdge;

   smallEdge.p1.X = smallEdge.p1.X + (smallEdge.normal.X * EXTRUSION_WIDTH * -1);
   smallEdge.p1.Y = smallEdge.p1.Y + (smallEdge.normal.Y * EXTRUSION_WIDTH * -1);


   smallEdge.p2.X = smallEdge.p2.X + (smallEdge.normal.X * EXTRUSION_WIDTH * -1);
   smallEdge.p2.Y = smallEdge.p2.Y + (smallEdge.normal.Y * EXTRUSION_WIDTH * -1);


   printf("\nINPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",bigEdge.p1.X,bigEdge.p1.Y,bigEdge.p2.X,bigEdge.p2.Y,bigEdge.normal.X,bigEdge.normal.Y,bigEdge.normal.Z);

   printf("\nOUTPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",smallEdge.p1.X,smallEdge.p1.Y,smallEdge.p2.X,smallEdge.p2.Y,smallEdge.normal.X,smallEdge.normal.Y,smallEdge.normal.Z);



   return smallEdge;

}














struct edge* shrinkLoops(struct edge* layerEdges, int layerEdgesI, int* edgesPerLoop, int numLoops){

 

   struct edge* newLoops = (struct edge*)calloc(sizeof(struct edge),layerEdgesI*layerEdgesI);


      //SHRINK ALL LOOPS
      for(int i = 0; i < numLoops; i++){

         for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

            newLoops[i * layerEdgesI + i2] = scaleEdgeInwards(layerEdges[i * layerEdgesI + i2]);

         }

      }
/*
      printf("ALL LOOP SHRINKING FINSISHED. FINDING NEW INTERSECTIONS\n");

      //FIND NEW INTERSECTIONS
      for(int i = 0; i < numLoops; i++){
      
         for(int i2 = 0; i2 < edgesPerLoop[i]; i2++){

           printf("Loop - i: %d, i2: %d\n",i,i2);


           int edge1Index = i * layerEdgesI + i2;
           int edge2Index = i * layerEdgesI + ((edgesPerLoop[i] - 1 + i2)%edgesPerLoop[i]);

           struct point temp = intersection(newLoops[edge1Index], newLoops[edge2Index]);

           printf("EDGE1: %f, %f ---- %f, %f\n",newLoops[edge1Index].p1.X,newLoops[edge1Index].p1.Y,newLoops[edge1Index].p2.X,newLoops[edge1Index].p2.Y);
           printf("EDGE2: %f, %f ---- %f, %f\n",newLoops[edge2Index].p1.X,newLoops[edge2Index].p1.Y,newLoops[edge2Index].p2.X,newLoops[edge2Index].p2.Y);


           //IF THERE IS NO INTERSECTION BETWEEN EDGE I and I+1
           if(temp.X == FLT_MAX && temp.Y == FLT_MAX){

              printf("\n\nNO INTERSECTION ERROR!!! BIG BAD\n\n");

              temp.X = 0.0;
              temp.Y = 0.0;

           }


           //IF POINT 1 CONNECTS TO POINT 1 OF THE NEXT EDGE
           if(layerEdges[edge1Index].p1.X == layerEdges[edge2Index].p1.X && layerEdges[edge1Index].p1.Y == layerEdges[edge2Index].p1.Y){

              layerEdges[edge1Index].p1 = temp;
              layerEdges[edge2Index].p1 = temp;
              continue;

           }

           //IF POINT 1 CONNECTS TO POINT 2 OF THE NEXT EDGE
           if(layerEdges[edge1Index].p1.X == layerEdges[edge2Index].p2.X && layerEdges[edge1Index].p1.Y == layerEdges[edge2Index].p2.Y){

              layerEdges[edge1Index].p1 = temp;
              layerEdges[edge2Index].p2 = temp;
              continue;

           }


           //IF POINT 2 CONNECTS TO POINT 1 OF THE NEXT EDGE
           if(layerEdges[edge1Index].p2.X == layerEdges[edge2Index].p1.X && layerEdges[edge1Index].p2.Y == layerEdges[edge2Index].p1.Y){

              layerEdges[edge1Index].p2 = temp;
              layerEdges[edge2Index].p1 = temp;
              continue;

           }


           //IF POINT 2 CONNECTS TO POINT 2 OF THE NEXT EDGE
           if(layerEdges[edge1Index].p2.X == layerEdges[edge2Index].p2.X && layerEdges[edge1Index].p2.Y == layerEdges[edge2Index].p2.Y){

              layerEdges[edge1Index].p2 = temp;
              layerEdges[edge2Index].p2 = temp;
              continue;

           }







         }

      }


*/
   return newLoops;
  

}





//RETURNED POINTER MUST BE FREE'D
//edgesPerLoop ARG POINTER MUST BE FREE'D

struct edge* edgesToLoops(struct edge* layerEdges, int layerEdgesI, struct point currentPoint, int** edgesPerLoop, int* currentLoop){

     
   //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

   // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


   float lowestDistance = -1;

   int startEdge = 0;

   for(int i = 0; i < layerEdgesI; i++){

      if(pointDistance(layerEdges[i].p1, currentPoint) < lowestDistance || lowestDistance < 0){

         lowestDistance = pointDistance(layerEdges[i].p1, currentPoint);

         startEdge = i;

      }

   }



   //MOVE TO NEAREST POINT ON THIS LAYER

   currentPoint.X = layerEdges[startEdge].p1.X;
   currentPoint.Y = layerEdges[startEdge].p1.Y;






   /////////////////////////////
   //SEPARATE EDGES INTO LOOPS//
   /////////////////////////////

   //List of loops which consist of lists of edges
   //Max number of loops is the number of edges divided by 3 because a loop must be at least 3 edges
   struct edge* loops = (struct edge*)calloc(sizeof(struct edge),layerEdgesI*layerEdgesI);

   (*edgesPerLoop) = calloc(sizeof(int),layerEdgesI);
   *currentLoop = 0;
   int loopI = 0;



   //List of used edges from layerEdges
   int edgesUsed[layerEdgesI];

   int numEdgesUsed = 0;





   while(numEdgesUsed < layerEdgesI){
      
      int edgeAddedThisLoop = 0;

      for(int i = 0; i < layerEdgesI; i++){


         //CHECK IF CURRENTLY CONSIDERED EDGE IS ALREADY USED
         int edgeIsUsed = 0;

         for(int i2 = 0; i2 < numEdgesUsed; i2++){

            if(i == edgesUsed[i2]){

               edgeIsUsed = 1;
            }

         }



         //CHECK IF EDGE STARTS WITH OUR CURRENT POINT - IF SO ADD TO LOOP
         if(edgeIsUsed == 0 && layerEdges[i].p1.X == currentPoint.X && layerEdges[i].p1.Y == currentPoint.Y){


            int temp =  ((*currentLoop) * layerEdgesI) + ((*edgesPerLoop)[(*currentLoop)]);


            loops[temp] = layerEdges[i];


            (*edgesPerLoop)[(*currentLoop)] += 1;


            edgesUsed[numEdgesUsed] = i;
            numEdgesUsed++;

            currentPoint.X = layerEdges[i].p2.X;
            currentPoint.Y = layerEdges[i].p2.Y;


            edgeAddedThisLoop = 1;

         }


      }


      if(edgeAddedThisLoop == 0){
         
         (*currentLoop) += 1;




     
         //FIND THE NEAREST EDGE TO CURRENT POSITION TO START PRINTING ON

         // ##POSSIBLE ISSUE## WILL ONLY CHECK THE FIRST POINT OF THE EDGE ##POSSIBLE ISSUE##


         lowestDistance = -1;

         startEdge = 0;

         for(int i = 0; i < layerEdgesI; i++){



            //CHECK IF CURRENTLY CONSIDERED EDGE IS ALREADY USED
            int edgeIsUsed = 0;

            for(int i2 = 0; i2 < numEdgesUsed; i2++){

               if(i == edgesUsed[i2]){

                  edgeIsUsed = 1;

               }

            }




            if(edgeIsUsed == 0 && (pointDistance(layerEdges[i].p1, currentPoint) < lowestDistance || lowestDistance < 0)){

               lowestDistance = pointDistance(layerEdges[i].p1, currentPoint);

               startEdge = i;

            }

         }


         currentPoint.X = layerEdges[startEdge].p1.X;
         currentPoint.Y = layerEdges[startEdge].p1.Y;


      }



   }

   //ONE EXTRA ITERATION TO currentLoop BECAUSE IT WILL MISS ONE IN ABOVE ALGORITHM

         
   (*currentLoop) += 1;




   ////////////////////////////////////
   //WRITE PERIMETER EDGES FROM LOOPS//
   ////////////////////////////////////

   printf("\nEDGESPERLOOP[0]: %d\n",(*edgesPerLoop)[0]);

   for(int i = 0; i < (*currentLoop); i++){

      for(int i2 = 0; i2 < (*edgesPerLoop)[i]; i2++){

         printf("LOOP: %d, EDGE: %d - %f, %f TO %f, %f\n",i,i2,loops[(i * layerEdgesI) + i2].p1.X, loops[(i * layerEdgesI) + i2].p1.Y, loops[(i * layerEdgesI) + i2].p2.X, loops[(i * layerEdgesI) + i2].p2.Y);

      }

   }




   return loops;

}


















//RETURNED POINTER MUST BE FREE'D
struct edge* slice(struct tri* triangles, int numTriangles, float currentHeight, int *numEdges){







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
























   //not sure what to set the maximum number of possible layer vertices as
   struct point layerPoints[numTriangles];
   int layerPointsI = 0;











   if(currentHeight > maxHeight && ((currentHeight - maxHeight) < (maxHeight - (currentHeight - LAYER_HEIGHT)))){

      printf("\n\nFINAL CONDITIONAL LAYER\n\n");






      //FIND ALL POINTS THAT LIE ON TOP LAYER
      for(int i = 0; i < numTriangles; i++){


         if(triangles[i].p1.Z == maxHeight){

            
            struct point temp1 = triangles[i].p1;
            temp1.Z = currentHeight;
            addUniquePoint(temp1, layerPoints, &layerPointsI);


         }


         if(triangles[i].p2.Z == maxHeight){
            
            struct point temp2 = triangles[i].p2;
            temp2.Z = currentHeight;
            addUniquePoint(temp2, layerPoints, &layerPointsI);


         }


         if(triangles[i].p3.Z == maxHeight){

            struct point temp3 = triangles[i].p3;
            temp3.Z = currentHeight;
            addUniquePoint(temp3, layerPoints, &layerPointsI);

         }


      } 






      printf("%d Points\n",layerPointsI);


      for(int i = 0; i < layerPointsI; i++){

         printf("X: %f, Y: %f, Z: %f\n",layerPoints[i].X,layerPoints[i].Y,layerPoints[i].Z);

         for(int i2 = 0; i2 < layerPointsI; i2++){

            if(layerPoints[i].X == layerPoints[i2].X && layerPoints[i].Y == layerPoints[i2].Y && layerPoints[i].Z == layerPoints[i2].Z && i2 != i){

               printf("Duplicate points!\n");

            }

         }

      }






      return pointsToEdges(layerPoints, layerPointsI, triangles, numTriangles, numEdges);


   //END OF CONDITIONAL FINAL LAYER IF
   }else{












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


      return pointsToEdges(layerPoints, layerPointsI, triangles, numTriangles, numEdges);



   }




}














