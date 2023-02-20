#define FLOAT_ERROR 0.0000000000000001


struct point{

   float X;
   float Y;
   float Z;

};


struct edge{

   struct point p1;
   struct point p2;

   struct point normal;

};



struct tri{

   struct point p1;
   struct point p2;
   struct point p3;

   struct point normal;

};

//SWAPS THE POINTS OF A GIVEN EDGE
//DOES NOT CHANGE NORMAL
struct edge swapEdgePoints(struct edge e){

   struct point temp;

   temp.X = e.p1.X;
   temp.Y = e.p1.Y;
   temp.Z = e.p1.Z;

   e.p1.X = e.p2.X;
   e.p1.Y = e.p2.Y;
   e.p1.Z = e.p2.Z;

   e.p2.X = temp.X;
   e.p2.Y = temp.Y;
   e.p2.Z = temp.Z;

   return e;

}


//USED TO COMPARE TWO FLOAT VALUES
//PREVENTS ISSUES DUE TO FLOATING POINT MATH WEIRDNESS
int floatIsEqual(float f1, float f2){

   if(fabs(f1 - f2) < FLOAT_ERROR){

      return 1;

   }

   return 0;

}



//RETURNS DISTANCE BETWEEN TWO POINTS ON THE XY PLANE
float pointDistance(struct point p1, struct point p2){


   float dX = p1.X - p2.X;
   float dY = p1.Y - p2.Y;
   float dZ = p1.Z - p2.Z;

   dX = dX*dX;
   dY = dY*dY;
   dZ = dZ*dZ;

   dX = dX+dY+dZ;
   dX = sqrt(dX);


   return dX;

}








struct point normal3Dto2D(struct point normal3D){

   struct point zero;
   zero.X = 0;
   zero.Y = 0;
   zero.Z = 0;


   struct point normal2D;
   normal2D.X = normal3D.X;
   normal2D.Y = normal3D.Y;
   normal2D.Z = 0.0;


   float magnitude = pointDistance(zero, normal2D);


   normal2D.X = (normal2D.X / magnitude == 0) ? 0 : (normal2D.X / magnitude);

   normal2D.Y = (normal2D.Y / magnitude == 0) ? 0 : (normal2D.Y / magnitude);

   normal2D.Z = 0.0;


   return normal2D;


}






//SHRINKS EDGE TOWARDS ITS CENTER BY A SET AMOUNT ON EITHER SIDE
//2D FUNCTION -- RETURNS IDENTICAL Z VALUES TO INPUG EDGE
//RETURNS IDENTICAL NORMAL ON EDGE BUT ONLY CONSIDERS 2D NORMAL
struct edge shrinkEdge(struct edge edgeIn, float shrinkLength){

   struct edge shrunkEdge;

   shrunkEdge.normal = normal3Dto2D(edgeIn.normal);


   if(edgeIn.p1.X > edgeIn.p2.X){

      shrunkEdge.p1.X = edgeIn.p1.X - fabs(shrunkEdge.normal.Y * shrinkLength);
      shrunkEdge.p2.X = edgeIn.p2.X + fabs(shrunkEdge.normal.Y * shrinkLength);

   }else{

      shrunkEdge.p1.X = edgeIn.p1.X + fabs(shrunkEdge.normal.Y * shrinkLength);
      shrunkEdge.p2.X = edgeIn.p2.X - fabs(shrunkEdge.normal.Y * shrinkLength);

   }

   if(edgeIn.p1.Y > edgeIn.p2.Y){

      shrunkEdge.p1.Y = edgeIn.p1.Y - fabs(shrunkEdge.normal.X * shrinkLength);
      shrunkEdge.p2.Y = edgeIn.p2.Y + fabs(shrunkEdge.normal.X * shrinkLength);

   }else{

      shrunkEdge.p1.Y = edgeIn.p1.Y + fabs(shrunkEdge.normal.X * shrinkLength);
      shrunkEdge.p2.Y = edgeIn.p2.Y - fabs(shrunkEdge.normal.X * shrinkLength);

   }


   shrunkEdge.p1.Z = edgeIn.p1.Z;
   shrunkEdge.p2.Z = edgeIn.p2.Z;

   shrunkEdge.normal.X = edgeIn.normal.X;
   shrunkEdge.normal.Y = edgeIn.normal.Y;
   shrunkEdge.normal.Z = edgeIn.normal.Z;


   return shrunkEdge;

}








struct edge scaleEdgeInwards(struct edge bigEdge, float distIn){

   struct edge smallEdge = bigEdge;

   smallEdge.normal = normal3Dto2D(bigEdge.normal);

   smallEdge.p1.X = smallEdge.p1.X + (smallEdge.normal.X * distIn * -1);
   smallEdge.p1.Y = smallEdge.p1.Y + (smallEdge.normal.Y * distIn * -1);


   smallEdge.p2.X = smallEdge.p2.X + (smallEdge.normal.X * distIn * -1);
   smallEdge.p2.Y = smallEdge.p2.Y + (smallEdge.normal.Y * distIn * -1);

/*

   printf("\nINPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",bigEdge.p1.X,bigEdge.p1.Y,bigEdge.p2.X,bigEdge.p2.Y,bigEdge.normal.X,bigEdge.normal.Y,bigEdge.normal.Z);

   printf("\nOUTPUT EDGE:\n\n");
   printf("X: %f Y: %f -- X: %f Y: %f, NORMAL: %f,%f,%f",smallEdge.p1.X,smallEdge.p1.Y,smallEdge.p2.X,smallEdge.p2.Y,smallEdge.normal.X,smallEdge.normal.Y,smallEdge.normal.Z);

*/


   return smallEdge;

}





int triFacesUp(struct tri t){

   if(t.normal.X == 0 && t.normal.Y == 0 && t.normal.Z == 1){
      
      return 1;

   }


   return 0;

}



int triFacesDown(struct tri t){


   if(t.normal.X == 0 && t.normal.Y == 0 && t.normal.Z == -1){
      
      return 1;

   }



   return 0;

}




struct point computeNormal(struct tri Triangle){

   struct point temp;


   struct point zero;
   zero.X = 0;
   zero.Y = 0;
   zero.Z = 0;



   struct point BminusA;

   BminusA.X = Triangle.p3.X - Triangle.p1.X;
   BminusA.Y = Triangle.p3.Y - Triangle.p1.Y;
   BminusA.Z = Triangle.p3.Z - Triangle.p1.Z;



   struct point BminusC;

   BminusC.X = Triangle.p2.X - Triangle.p1.X;
   BminusC.Y = Triangle.p2.Y - Triangle.p1.Y;
   BminusC.Z = Triangle.p2.Z - Triangle.p1.Z;




   temp.X = (BminusA.Z * BminusC.Y) - (BminusA.Y * BminusC.Z);
   temp.Y = (BminusA.X * BminusC.Z) - (BminusA.Z * BminusC.X);
   temp.Z = (BminusA.Y * BminusC.X) - (BminusA.X * BminusC.Y);


   float magnitude = pointDistance(zero, temp);

   temp.X = temp.X / magnitude == 0 ? 0 : temp.X / magnitude;

   temp.Y = temp.Y / magnitude == 0 ? 0 : temp.Y / magnitude;

   temp.Z = temp.Z / magnitude == 0 ? 0 : temp.Z / magnitude;



   return temp;

}




struct point intersection(struct edge edge1, struct edge edge2){

   struct point intersectPoint;



   float t = ((edge1.p1.X - edge2.p1.X) * (edge2.p1.Y - edge2.p2.Y)) - ((edge1.p1.Y - edge2.p1.Y) * (edge2.p1.X - edge2.p2.X));

   t = t / (((edge1.p1.X - edge1.p2.X) * (edge2.p1.Y - edge2.p2.Y)) - ((edge1.p1.Y - edge1.p2.Y) * (edge2.p1.X - edge2.p2.X)));



   float u = ((edge1.p1.X - edge2.p1.X) * (edge1.p1.Y - edge1.p2.Y)) - ((edge1.p1.Y - edge2.p1.Y) * (edge1.p1.X - edge1.p2.X));

   u = u / (((edge1.p1.X - edge1.p2.X) * (edge2.p1.Y - edge2.p2.Y)) - ((edge1.p1.Y - edge1.p2.Y) * (edge2.p1.X - edge2.p2.X)));



   if(t >= 0 && t <= 1 && u >= 0 && u <= 1){

      intersectPoint.X = edge1.p1.X + ( t * (edge1.p2.X - edge1.p1.X));
      intersectPoint.Y = edge1.p1.Y + ( t * (edge1.p2.X - edge1.p1.X));

   }else{

      intersectPoint.X = FLT_MAX;
      intersectPoint.Y = FLT_MAX;

   }





   return intersectPoint;

}



//This generates normals but they are inverted

/*
struct point computeNormal(struct tri Triangle){

   struct point temp;

   struct point zero;
   zero.X = 0;
   zero.Y = 0;
   zero.Z = 0;


   //Dir = (B - A) x (C - A)
   //Norm = Dir / len(Dir)


   struct point BminusA;

   BminusA.X = Triangle.p2.X - Triangle.p1.X;
   BminusA.Y = Triangle.p2.Y - Triangle.p1.Y;
   BminusA.Z = Triangle.p2.Z - Triangle.p1.Z;



   struct point BminusC;

   BminusC.X = Triangle.p2.X - Triangle.p3.X;
   BminusC.Y = Triangle.p2.Y - Triangle.p3.Y;
   BminusC.Z = Triangle.p2.Z - Triangle.p3.Z;


   //cx = aybz − azby
   //cy = azbx − axbz
   //cz = axby − aybx



   temp.X = (BminusA.Y * BminusC.Z) - (BminusA.Z * BminusC.Y);
   temp.Y = (BminusA.Z * BminusC.X) - (BminusA.X * BminusC.Z);
   temp.Z = (BminusA.X * BminusC.Y) - (BminusA.Y * BminusC.X);


   float magnitude = pointDistance(zero, temp);

   temp.X = temp.X / magnitude;

   temp.Y = temp.Y / magnitude;

   temp.Z = temp.Z / magnitude;




   return temp;

}

*/


