
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


   normal2D.X = normal2D.X / magnitude == 0 ? 0 : normal2D.X / magnitude;

   normal2D.Y = normal2D.Y / magnitude == 0 ? 0 : normal2D.Y / magnitude;


   return normal2D;


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


