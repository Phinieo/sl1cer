
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

   float dist = 0;



   float dX = p1.X - p2.X;
   float dY = p1.Y - p2.Y;

   dX = dX*dX;
   dY = dY*dY;

   dX = dX+dY;
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









