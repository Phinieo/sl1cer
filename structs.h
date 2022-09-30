
struct point{

   float X;
   float Y;
   float Z;

};


struct edge{

   struct point p1;
   struct point p2;

};



struct tri{

   struct point p1;
   struct point p2;
   struct point p3;

   struct point normal;

};




struct point centerPoint(struct point p){

   struct point temp;


   temp.X = p.X + (MAX_X/2);
   temp.Y = p.Y + (MAX_Y/2);
   temp.Z = p.Z;


   return temp;

}





struct tri centerTriangle(struct tri t){

   struct tri temp;

   temp.p1 = centerPoint(t.p1);
   temp.p2 = centerPoint(t.p2);
   temp.p3 = centerPoint(t.p3);


   return temp;

}




struct edge centerEdge(struct edge e){

   struct edge temp;

   temp.p1 = centerPoint(e.p1);
   temp.p2 = centerPoint(e.p2);

   return temp;

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









