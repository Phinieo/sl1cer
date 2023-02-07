
//RETURNED POINTER MUST BE FREE'D
struct tri* readSTL(char* filename, unsigned int* numTriangles){

   FILE* fp;

   char wordIn = '\0'; 


   fp = fopen(filename, "rb");





   //READ 80 BYTE HEADER
   for(int i = 0; i < 80; i++){

      fread(&wordIn, sizeof(char), 1, fp);

   }


   //READ 4 BYTE NUMBER OF TRIANGLES
   fread(numTriangles, sizeof(char), 4, fp);

   printf("\n\n\n\n%d TRIANGLES\n\n\n\n\n",*numTriangles);


   //READ ALL TRIANGLES
   struct tri* triangles = calloc(*numTriangles, sizeof(struct tri));

   for(int i = 0; i < *numTriangles; i++){

      //READ NORMAL VECTOR
      fread(&(triangles[i].normal.X), sizeof(char), 4, fp);
      fread(&(triangles[i].normal.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].normal.Z), sizeof(char), 4, fp);

      //READ X
      fread(&(triangles[i].p1.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p1.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p1.Z), sizeof(char), 4, fp);

      //READ Y
      fread(&(triangles[i].p2.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p2.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p2.Z), sizeof(char), 4, fp);

      //READ Z
      fread(&(triangles[i].p3.X), sizeof(char), 4, fp);
      fread(&(triangles[i].p3.Y), sizeof(char), 4, fp);
      fread(&(triangles[i].p3.Z), sizeof(char), 4, fp);

      //READ THROW-AWAY
      int throwaway;
      fread(&throwaway, sizeof(char), 2, fp);

      
      printf("FILE NORMAL: %f, %f, %f\n", triangles[i].normal.X, triangles[i].normal.Y, triangles[i].normal.Z);

      struct point Normal = computeNormal(triangles[i]);
      printf("Computed NORMAL: %f, %f, %f\n\n", Normal.X, Normal.Y, Normal.Z);


   }


   fclose(fp);
 

   return triangles;

}

