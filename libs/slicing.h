


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

   //printf("\nADDING UNIQUE EDGE: %f, %f to %f, %f\n",e.p1.X, e.p1.Y, e.p2.X, e.p2.Y);


   return 1;



}





int countLoops(struct edge* edges, int numEdges){

   //SOLVES EDGE CASE IN WHICH THERE ARE NO LOOPS
   if(numEdges == 0){

      return 0;

   }

   //RETURN VALUE TO BE COMPUTED
   int numLoops = 0;


   struct point currentPoint;

   currentPoint.X = edges[0].p1.X;
   currentPoint.Y = edges[0].p1.Y;
   currentPoint.Z = edges[0].p1.Z;


   //STRUCTURE TO SAVE THE COUNTED EDGES IN
   struct edge* usedEdges = calloc(numEdges, sizeof(struct edge));
   int numUsedEdges = 0;


   int loopOngoingFlag = 0;

   //CONTINUE FINDING LOOPS UNTILL ALL EDGES ARE USED
   while(numUsedEdges < numEdges){

      loopOngoingFlag = 0;

      for(int i = 0; i < numEdges; i++){

         //printf("\nCOMPARING CURRENT %f, %f TO %f, %f",currentPoint.X,currentPoint.Y,edges[i].p1.X,edges[i].p1.Y);
         //printf(" -- RESULT: %d\n\n",currentPoint.X == edges[i].p1.X && currentPoint.Y == edges[i].p1.Y, currentPoint.Z == edges[i].p1.Z);

         //ADD A CONNECTED EDGE
         if(currentPoint.X == edges[i].p1.X && currentPoint.Y == edges[i].p1.Y && currentPoint.Z == edges[i].p1.Z){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               loopOngoingFlag += 1;

               //printf("\nBLOCK 1 - NEW CURRENT POINT: %f, %f\n\n",currentPoint.X,currentPoint.Y);

               break;

            }

         //ADD A REVERSED EDGE
         }else if(currentPoint.X == edges[i].p2.X && currentPoint.Y == edges[i].p2.Y && currentPoint.Z == edges[i].p2.Z){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p1.X;
               currentPoint.Y = edges[i].p1.Y;
               currentPoint.Z = edges[i].p1.Z;

               loopOngoingFlag += 1;

               //printf("\nBLOCK 2 - NEW CURRENT POINT: %f, %f\n\n",currentPoint.X,currentPoint.Y);

               break;

            }

         }


      }

      //IF LOOP HAS ENDED
      //TRIGGERS IF EACH POINT IN INPUT EDGE LIST IS CHECKED AND NONE ARE PART OF THE CURRENT LOOP
      if(loopOngoingFlag == 0){

         numLoops++;

         //FINDS THE AN EDGE THAT IS NOT IN ANY PREVIOUS LOOPS
         //FINDS THE START OF A NEW LOOP
         for(int i = 0; i < numEdges; i++){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               break;

            }

         }



      }


   }

   free(usedEdges);
   
   numLoops++;

   return numLoops;

}







struct edge* getLoop(struct edge* edges, int numEdges, int targetLoop, int* targetLoopSize){

   //SOLVES EDGE CASE IN WHICH THERE ARE NO LOOPS
   if(numEdges == 0){

      return 0;

   }

   //USED TO DETERMINE WHICH LOOP IS BEING COMPUTED
   //COMPARED TO targetLoop
   int numLoops = 0;


   //WILL REPRESENT THE STARTING INDECIE OF THE TARGET LOOP INSIDE usedEdges
   int targetLoopStart = -1;


   struct point currentPoint;

   currentPoint.X = edges[0].p1.X;
   currentPoint.Y = edges[0].p1.Y;
   currentPoint.Z = edges[0].p1.Z;


   //STRUCTURE TO SAVE THE COUNTED EDGES IN
   struct edge* usedEdges = calloc(numEdges, sizeof(struct edge));
   int numUsedEdges = 0;


   int loopOngoingFlag = 0;

   //CONTINUE FINDING LOOPS UNTILL ALL EDGES ARE USED
   while(numUsedEdges < numEdges){



      if(numLoops == targetLoop && targetLoopStart == -1){

        //IF THE FIRST LOOP IS THE TARGET LOOP START AT 0
        if(numUsedEdges == 0){

           targetLoopStart = numUsedEdges;

        //IF THE TARGET LOOP IS A DIFFERENT LOOP THEN START AT 1 LESS THAN THE NUMBER OF USED EDGES
        //THE LOOPS FIRST EDGE IS ADDED BEFORE THIS CHECK AND numUsedEdges GETS ITERATED EARLY
        }else{

           targetLoopStart = numUsedEdges - 1;

        }
        //printf("REACHED TARGET LOOP\n");

      }



      loopOngoingFlag = 0;

      for(int i = 0; i < numEdges; i++){

         //printf("\nCOMPARING CURRENT %f, %f TO %f, %f",currentPoint.X,currentPoint.Y,edges[i].p1.X,edges[i].p1.Y);
         //printf(" -- RESULT: %d\n\n",currentPoint.X == edges[i].p1.X && currentPoint.Y == edges[i].p1.Y, currentPoint.Z == edges[i].p1.Z);

         //ADD A CONNECTED EDGE
         if(currentPoint.X == edges[i].p1.X && currentPoint.Y == edges[i].p1.Y && currentPoint.Z == edges[i].p1.Z){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               loopOngoingFlag += 1;

               //printf("\nBLOCK 1 - NEW CURRENT POINT: %f, %f\n\n",currentPoint.X,currentPoint.Y);

               break;

            }

         //ADD A REVERSED EDGE
         }else if(currentPoint.X == edges[i].p2.X && currentPoint.Y == edges[i].p2.Y && currentPoint.Z == edges[i].p2.Z){

            if(addUniqueEdge(swapEdgePoints(edges[i]), usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p1.X;
               currentPoint.Y = edges[i].p1.Y;
               currentPoint.Z = edges[i].p1.Z;

               loopOngoingFlag += 1;

               //printf("\nBLOCK 2 - NEW CURRENT POINT: %f, %f\n\n",currentPoint.X,currentPoint.Y);

               break;

            }

         }


      }


      //IF LOOP HAS ENDED
      //TRIGGERS IF EACH POINT IN INPUT EDGE LIST IS CHECKED AND NONE ARE PART OF THE CURRENT LOOP
      if(loopOngoingFlag == 0){

         //RETURN CONDITION
         //ENTERS IF WE HAVE FOUND THE START OF THE TARGET LOOP
         //ABOVE IF ALSO GUARUNTEES WE HAVE FINISHED COMPUTING THAT LOOP
         if(targetLoopStart != -1){

            struct edge* temp = calloc(numUsedEdges - targetLoopStart, sizeof(struct edge));
            *targetLoopSize = 0;


            for(int i = 0; i < (numUsedEdges - targetLoopStart); i++){

               addUniqueEdge(usedEdges[targetLoopStart + i], temp, targetLoopSize);

            }


            free(usedEdges);

            return temp;

         }

         numLoops++;


         //FINDS THE AN EDGE THAT IS NOT IN ANY PREVIOUS LOOPS
         //FINDS THE START OF A NEW LOOPb
         for(int i = 0; i < numEdges; i++){

            if(addUniqueEdge(edges[i], usedEdges, &numUsedEdges)){

               currentPoint.X = edges[i].p2.X;
               currentPoint.Y = edges[i].p2.Y;
               currentPoint.Z = edges[i].p2.Z;

               break;

            }

         }



      }


   }


   //RETURN LAST FOUND LOOP IF SELECTED LOOP IS AT THE END OF THE INPUT LIST OF EDGES

   //RETURN CONDITION
   //ENTERS IF WE HAVE FOUND THE START OF THE TARGET LOOP
   //ABOVE IF ALSO GUARUNTEES WE HAVE FINISHED COMPUTING THAT LOOP
   if(targetLoopStart != -1){

      struct edge* temp = calloc(numUsedEdges - targetLoopStart, sizeof(struct edge));
      *targetLoopSize = 0;


      for(int i = 0; i < (numUsedEdges - targetLoopStart); i++){

         addUniqueEdge(usedEdges[targetLoopStart + i], temp, targetLoopSize);

      }


      free(usedEdges);

      return temp;

   }


   printf("ERROR: TRIED TO FIND LOOP THAT DID NOT EXIST");
   return NULL;


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

                     //printf("\n\nEDGE: %f,%f and %f,%f",layerEdges[*layerEdgesI-1].p1.X,layerEdges[*layerEdgesI-1].p1.Y,layerEdges[*layerEdgesI-1].p2.X,layerEdges[*layerEdgesI-1].p2.Y);
 
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

                     //printf("\n\nEDGE: %f,%f and %f,%f",layerEdges[*layerEdgesI-1].p1.X,layerEdges[*layerEdgesI-1].p1.Y,layerEdges[*layerEdgesI-1].p2.X,layerEdges[*layerEdgesI-1].p2.Y);
 
                  }

               }

            

            }



         }



      }

   }



   printf("\n\nEDGES: %d\n\n",*layerEdgesI);


   return layerEdges;

}







//SHRINKS A LOOP BY COMPUTING POINT NORMALS AND MOVING THEM INWARDS BY AN AMOUNT
//RETURNED POINTER MUST BE FREE'D
//INPUT loopEdges IS FREE'D IN FUNCTION

struct edge* shrinkLoop(struct edge* loopEdges, int numLoopEdges, float shrinkDist){


   struct edge* newLoop = (struct edge*)calloc(sizeof(struct edge),numLoopEdges);

   for(int i = 0; i < numLoopEdges; i++){

      struct point pointNormal;
      
      //COMBINE THE NORMALS OF CURRENT EDGE AND NEXT EDGE
      //NEXT EDGE IS MOD numLoopEdges TO WRAP AROUND TO FIRST
      //CONVERTED TO 2D NORMAL VECTOR ON XY PLANE
      pointNormal = normal3Dto2D(combineNormals(loopEdges[i].normal, loopEdges[( (i + 1) % numLoopEdges )].normal));

      //MODIFY CURRENT EDGES POINT
      newLoop[i].p2.X = loopEdges[i].p2.X + (pointNormal.X * shrinkDist * -1);
      newLoop[i].p2.Y = loopEdges[i].p2.Y + (pointNormal.Y * shrinkDist * -1);
      newLoop[i].p2.Z = loopEdges[i].p2.Z + (pointNormal.Z * shrinkDist * -1);

      //MODIFY NEXT EDGES POINT (SHOULD BE SAME POINT AS ABOVE)
      newLoop[( (i + 1) % numLoopEdges )].p1.X = loopEdges[( (i + 1) % numLoopEdges )].p1.X + (pointNormal.X * shrinkDist * -1);
      newLoop[( (i + 1) % numLoopEdges )].p1.Y = loopEdges[( (i + 1) % numLoopEdges )].p1.Y + (pointNormal.Y * shrinkDist * -1);
      newLoop[( (i + 1) % numLoopEdges )].p1.Z = loopEdges[( (i + 1) % numLoopEdges )].p1.Z + (pointNormal.Z * shrinkDist * -1);
      
      newLoop[i].normal = loopEdges[i].normal;
   }

/*
   for(int i = 0; i < numLoopEdges; i++){

      printf("UNSHRK EDGE %d: %f, %f to %f, %f\n", i, loopEdges[i].p1.X,loopEdges[i].p1.Y,loopEdges[i].p2.X,loopEdges[i].p2.Y);
      printf("SHRUNK EDGE %d: %f, %f to %f, %f\n\n", i, newLoop[i].p1.X, newLoop[i].p1.Y, newLoop[i].p2.X, newLoop[i].p2.Y);

   }
*/

   return newLoop;
  

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





/*
      printf("%d Points\n",layerPointsI);


      for(int i = 0; i < layerPointsI; i++){

         printf("X: %f, Y: %f, Z: %f\n",layerPoints[i].X,layerPoints[i].Y,layerPoints[i].Z);

         for(int i2 = 0; i2 < layerPointsI; i2++){

            if(layerPoints[i].X == layerPoints[i2].X && layerPoints[i].Y == layerPoints[i2].Y && layerPoints[i].Z == layerPoints[i2].Z && i2 != i){

               printf("Duplicate points!\n");

            }

         }

      }
*/





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


/*
      printf("%d Points\n",layerPointsI);


      for(int i = 0; i < layerPointsI; i++){

         printf("X: %f, Y: %f, Z: %f\n",layerPoints[i].X,layerPoints[i].Y,layerPoints[i].Z);

         for(int i2 = 0; i2 < layerPointsI; i2++){

            if(layerPoints[i].X == layerPoints[i2].X && layerPoints[i].Y == layerPoints[i2].Y && layerPoints[i].Z == layerPoints[i2].Z && i2 != i){

               printf("Duplicate points!\n");

            }

         }

      }
*/




      //CONVERT POINTS TO EDGES


      return pointsToEdges(layerPoints, layerPointsI, triangles, numTriangles, numEdges);



   }




}

















//DOES NOT DO FINAL LAYER WHEN SLIGHTLY OVER MODEL'S TOP LAYER

//RETURNED POINTER MUST BE FREE'D
struct edge* sliceFaster(struct tri* triangles, int numTriangles, float currentHeight, int *layerEdgesI){




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



   int intersectedTriangleIndecies[numTriangles];
   int numIntersectedTriangles = 0;


   //FIND THE NUMBER OF INTERSECTED TRIANGLES
   for(int i = 0; i < numTriangles; i++){

      //ADD TRIANGLE TO INTERSECTED LIST IF ONE POINT IS EXACTLY ON A VERTICE
      if(!(triFacesDown(triangles[i]) || triFacesUp(triangles[i]))){

         if(triangles[i].p1.Z == currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }

         if(triangles[i].p2.Z == currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }

         if(triangles[i].p3.Z == currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }


      }


      //FIND A POINT BELOW THE CURRENT LAYER
      if(triangles[i].p1.Z < currentHeight){


         if(triangles[i].p2.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }

         if(triangles[i].p3.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }


      }

      //FIND A POINT BELOW THE CURRENT LAYER
      if(triangles[i].p2.Z < currentHeight){


         if(triangles[i].p1.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }

         if(triangles[i].p3.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }


      }

      //FIND A POINT BELOW THE CURRENT LAYER
      if(triangles[i].p3.Z < currentHeight){


         if(triangles[i].p1.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }

         if(triangles[i].p2.Z > currentHeight){

            intersectedTriangleIndecies[numIntersectedTriangles] = i;
            numIntersectedTriangles++;
            continue;

         }


      }



   }


   //LAYER EDGES ARRAY
   //MUST BE FREE'D
   struct edge* layerEdges = (struct edge*)calloc(sizeof(struct edge),numIntersectedTriangles);
   (*layerEdgesI) = 0;


   //INTERSECTED TRIANGLES ARRAY
   //MUST BE FREE'D
   struct tri* layerTriangles = (struct tri*)calloc(sizeof(struct tri),numIntersectedTriangles);
   int layerTrianglesI = numIntersectedTriangles;

   //COPY OVER INTERSECTED TRIANGLES
   for(int i = 0; i < numIntersectedTriangles; i++){

      layerTriangles[i] = triangles[intersectedTriangleIndecies[i]];

   }


   
   for(int i = 0; i < layerTrianglesI; i++){

/*
      printf("TRIANGLE:\n\n");
      printf("P1: %f, %f, %f\n",layerTriangles[i].p1.X,layerTriangles[i].p1.Y,layerTriangles[i].p1.Z);
      printf("P2: %f, %f, %f\n",layerTriangles[i].p2.X,layerTriangles[i].p2.Y,layerTriangles[i].p2.Z);
      printf("P3: %f, %f, %f\n",layerTriangles[i].p3.X,layerTriangles[i].p3.Y,layerTriangles[i].p3.Z);
*/

      struct edge tempEdge;



      //IF P1 IS ABOVE AND P2, P3 ARE BELOW
      if(layerTriangles[i].p1.Z > currentHeight && layerTriangles[i].p2.Z < currentHeight && layerTriangles[i].p3.Z < currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p1, layerTriangles[i].p2);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p1, layerTriangles[i].p3);

         tempEdge.normal = layerTriangles[i].normal;
         
         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }

      //IF P2 IS ABOVE AND P1, P3 ARE BELOW
      if(layerTriangles[i].p2.Z > currentHeight && layerTriangles[i].p1.Z < currentHeight && layerTriangles[i].p3.Z < currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p2, layerTriangles[i].p1);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p2, layerTriangles[i].p3);

         tempEdge.normal = layerTriangles[i].normal;

         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }

      //IF P3 IS ABOVE AND P1, P2 ARE BELOW
      if(layerTriangles[i].p3.Z > currentHeight && layerTriangles[i].p1.Z < currentHeight && layerTriangles[i].p2.Z < currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p3, layerTriangles[i].p1);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p3, layerTriangles[i].p2);

         tempEdge.normal = layerTriangles[i].normal;

         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }




      //IF P1 IS BELOW AND P2, P3 ARE ABOVE
      if(layerTriangles[i].p1.Z < currentHeight && layerTriangles[i].p2.Z > currentHeight && layerTriangles[i].p3.Z > currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p1, layerTriangles[i].p2);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p1, layerTriangles[i].p3);

         tempEdge.normal = layerTriangles[i].normal;

         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }

      //IF P2 IS BELOW AND P1, P3 ARE ABOVE
      if(layerTriangles[i].p2.Z < currentHeight && layerTriangles[i].p1.Z > currentHeight && layerTriangles[i].p3.Z > currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p2, layerTriangles[i].p1);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p2, layerTriangles[i].p3);

         tempEdge.normal = layerTriangles[i].normal;

         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }

      //IF P3 IS BELOW AND P1, P2 ARE ABOVE
      if(layerTriangles[i].p3.Z < currentHeight && layerTriangles[i].p1.Z > currentHeight && layerTriangles[i].p2.Z > currentHeight){

         tempEdge.p1 = intersectLine(currentHeight, layerTriangles[i].p3, layerTriangles[i].p1);
         tempEdge.p2 = intersectLine(currentHeight, layerTriangles[i].p3, layerTriangles[i].p2);

         tempEdge.normal = layerTriangles[i].normal;

         //ADD THE GENERATED EDGE
         addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

         continue;

      }


      //FINDS EDGES THAT LIE ON THE EDGE OF A TRIANGLE
      //THERE IS SOME REPETITION OF EDGE CASES INCLUDED FOR CLARITY


      if(layerTriangles[i].p1.Z == currentHeight){

         //IF P1 AND P2 FORM AN EDGE
         if(layerTriangles[i].p2.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p1;
            tempEdge.p2 = layerTriangles[i].p2;

            tempEdge.normal = layerTriangles[i].normal;
            
            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;
         }

         //IF P1 AND P3 FORM AN EDGE
         if(layerTriangles[i].p3.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p1;
            tempEdge.p2 = layerTriangles[i].p3;

            tempEdge.normal = layerTriangles[i].normal;

            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;

         }

      }


      if(layerTriangles[i].p2.Z == currentHeight){

         //IF P2 AND P1 FORM AN EDGE
         if(layerTriangles[i].p1.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p2;
            tempEdge.p2 = layerTriangles[i].p1;

            tempEdge.normal = layerTriangles[i].normal;

            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;

         }

         //IF P2 AND P3 FORM AN EDGE
         if(layerTriangles[i].p3.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p2;
            tempEdge.p2 = layerTriangles[i].p3;

            tempEdge.normal = layerTriangles[i].normal;

            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;

         }

      }

      if(layerTriangles[i].p3.Z == currentHeight){

         //IF P3 AND P1 FORM AN EDGE
         if(layerTriangles[i].p1.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p3;
            tempEdge.p2 = layerTriangles[i].p1;

            tempEdge.normal = layerTriangles[i].normal;

            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;

         }

         //IF P3 AND P2 FORM AN EDGE
         if(layerTriangles[i].p2.Z == currentHeight){

            tempEdge.p1 = layerTriangles[i].p3;
            tempEdge.p2 = layerTriangles[i].p2;

            tempEdge.normal = layerTriangles[i].normal;

            //ADD THE GENERATED EDGE
            addUniqueEdge(tempEdge, layerEdges, layerEdgesI);

            continue;

         }

      }



   }

   //printf("LAYEREDGESI: %d\n\n",(*layerEdgesI));

   free(layerTriangles);

   return layerEdges;

}



