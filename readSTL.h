
struct point{

   float X;
   float Y;
   float Z;

};

struct tri{

   struct point p1;
   struct point p2;
   struct point p3;

   struct point normal;

};


struct tri* readSTL(char* filename, int* numTriangles){

   FILE* fp;

   char wordIn = '\0'; 


   fp = fopen(filename, "r");





   //READ 80 BYTE HEADER
   for(int i = 0; i < 80; i++){

      fread(&wordIn, sizeof(char), 1, fp);

   }


   //READ 4 BYTE NUMBER OF TRIANGLES
   fread(numTriangles, sizeof(char), 4, fp);




   //READ ALL TRIANGLES
   struct tri* triangles = calloc(*numTriangles, sizeof(struct tri));

   for(int i = 0; i < *numTriangles; i++){

      fread(&(triangles[i].normal.X), sizeof(char), 4, fp);
      fread(&(triangles[i].normal.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].normal.Z), sizeof(char), 4, fp);

      fread(&(triangles[i].p1.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p1.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p1.Z), sizeof(char), 4, fp);

      fread(&(triangles[i].p2.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p2.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p2.Z), sizeof(char), 4, fp);

      fread(&(triangles[i].p3.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p3.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p3.Z), sizeof(char), 4, fp);

      int throwaway;
      fread(&throwaway, sizeof(char), 2, fp);

   }


   fclose(fp);
 

   return triangles;

}

