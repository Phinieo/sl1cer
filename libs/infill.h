

struct edge* generateInfill(struct edge* perimeterEdges, int numPerimeterEdges){

   //IF INFILL_SPACING < 0 THEN NO INFILL IS NEEDED
   if(INFILL_SPACING < 0){

      return NULL;

   }

   //THE PREDICTED NUMBER OF INFILL EDGES
   //SCALES WITH SIZE OF OBJECT AND COMPLEXITY
   //SCALES INVERSELY WITH INFILL_SPACING
   int predictedNumInfillEdges = numPerimeterEdges*numPerimeterEdges * (11 - INFILL_SPACING);

   struct edge* infillEdges = calloc(predictedNumInfillEdges, sizeof(struct edge));
   
   int infillEdgesI = 0;


   for(int i = (-1*MAX_X/2); i < (MAX_X/2); i += INFILL_SPACING){

      struct point* intersections = calloc(numPerimeterEdges, sizeof(struct point));

      int intersectionsI = 0;

      
      //VERTICAL LINE OF POTENTIAL INFILL
      //WILL BE PRINTED IF INTERSECTS WITH OBJECT
      struct edge proposedInfill;

      proposedInfill.p1.X = i;
      proposedInfill.p1.Y = (-1*MAX_Y/2);

      proposedInfill.p2.X = i;
      proposedInfill.p2.Y = (MAX_Y/2);


      for(int i2 = 0; i2 < numPerimeterEdges; i2++){

         struct point temp = intersection(proposedInfill, perimeterEdges[i2]);

         if(temp.X != FLT_MAX && temp.Y != FLT_MAX){

            intersections[intersectionsI] = temp;
            intersectionsI++;

         }

      }

      printf("INFILL INTERSECTIONS X = %d: %d\n",i,intersectionsI);

      free(intersections);

   }

}