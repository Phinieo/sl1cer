
//POSSIBLE BUGS: COULD BE SOME WEIRDNESS WITH CHECKING FOR 'solid' STRING
//POSSIBLE BUGS: IN READING 5 BYTES, BYTE SIZE IS HARDCODED
int binaryOrAsciiSTL(char* filename){

   //OPEN FILE
   FILE* fp = fopen(filename, "rb");
   if(fp == NULL){

      printf("\n\nERROR: FILE COULD NOT BE OPENED\n\n");
      printf("--%s--\n",filename);
      return -1;

   }


   char fileStart[6];
   fileStart[5] = '\0';

   //READ FIRST 5 BYTES
   fread(&fileStart, 1, 5, fp);


   //IF THE FILE STARTS WTIH "solid" IT IS AN ASCII STL
   if(strstr(fileStart, "solid")){

      fclose(fp);
      return 0;

   }

   //IF THE FILE DOES NOT START WITH "solid" IT IS A BINARY STL
   fclose(fp);
   return 1;
   

}




//POSSIBLE BUGS: CASE SENSITVE TEXT
//RETURNED POINTER MUST BE FREE'D
struct tri* readAsciiSTL(char* filename, unsigned int* numTriangles){

   //OPEN FILE
   FILE* fp = fopen(filename, "rb");
   if(fp == NULL){

      printf("\n\nERROR: FILE COULD NOT BE OPENED\n\n");
      return NULL;

   }

   //VARIABLES FOR READING DATA BY LINE
   int maxLineLength = 1000;
   char line[maxLineLength];


   //DETERMINE NUMBER OF TRIANGLES IN FILE
   while(fgets(line, maxLineLength, fp)){

      if(strstr(line, "endfacet")){

         (*numTriangles)++;

      }

   }

   //RESET READ POSITION AFTER FINDING NUM TRIANGLES
   rewind(fp);

   //THROW AWAY FIRST LINE
   fgets(line, maxLineLength, fp);

   

   //RETURNED TRIANGLE DATA STRUCTURE
   struct tri* triangles = calloc(*numTriangles, sizeof(struct tri));
   int trianglesI = 0;

   //MAIN LOOP THAT GETS TRIANGLE DATA
   while(fgets(line, maxLineLength, fp) != NULL){


      //IF LINE IS "endsolid <name>\n" THE FILE IS DONE
      if(strstr(line, "endsolid") != NULL){

         printf("ASCII FILE READ SUCCESSFULLY\n");
         break;

      }


      //CHECKS FOR START OF TRIANGLE TO DETERMINE FILE VALIDITY
      if(strstr(line, "facet normal") == NULL){

         printf("\nERROR: ASCII FILE FORMAT IS WRONG.\n");

      }


      //THROW AWAY "facet"
      strtok(line, " ");

      //THROW AWAY "normal"
      strtok(NULL, " ");




      //GET NORMAL'S X VALUE
      triangles[trianglesI].normal.X = (float)atof(strtok(NULL, " "));

      //GET NORMAL'S Y VALUE
      triangles[trianglesI].normal.Y = (float)atof(strtok(NULL, " "));

      //GET NORMAL'S Z VALUE
      triangles[trianglesI].normal.Z = (float)atof(strtok(NULL, " "));


      //SKIP "outer loop\n" LINE
      fgets(line, maxLineLength, fp);






      //READ TRIANGLE VERTEX DATA
      //SAME CODE IS REPEATED FOR EACH TRIANGLE VERTICE

      //READ NEXT LINE
      fgets(line, maxLineLength, fp);

      //SKIP "vertex" text
      //ALSO INITIALIZE strtok() STATIC MEMORY TO NEW LINE
      strtok(line, " ");

      //READ IN X, Y, AND Z FOR VERTICE 1
      triangles[trianglesI].p1.X = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p1.Y = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p1.Z = (float)atof(strtok(NULL, " "));



      //READ NEXT LINE
      fgets(line, maxLineLength, fp);

      //SKIP "vertex" text
      //ALSO INITIALIZE strtok() STATIC MEMORY TO NEW LINE
      strtok(line, " ");

      //READ IN X, Y, AND Z FOR VERTICE 2
      triangles[trianglesI].p2.X = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p2.Y = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p2.Z = (float)atof(strtok(NULL, " "));



      //READ NEXT LINE
      fgets(line, maxLineLength, fp);

      //SKIP "vertex" text
      //ALSO INITIALIZE strtok() STATIC MEMORY TO NEW LINE
      strtok(line, " ");

      //READ IN X, Y, AND Z FOR VERTICE 3
      triangles[trianglesI].p3.X = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p3.Y = (float)atof(strtok(NULL, " "));
      triangles[trianglesI].p3.Z = (float)atof(strtok(NULL, " "));



      //SKIP "endloop\n" LINE
      fgets(line, maxLineLength, fp);
      //SKIP "endfacet\n" LINE
      fgets(line, maxLineLength, fp);


      trianglesI++;

   }

   fclose(fp);

   return triangles;

}




//RETURNED POINTER MUST BE FREE'D
struct tri* readBinarySTL(char* filename, unsigned int* numTriangles){

   FILE* fp;

   char wordIn = '\0'; 


   fp = fopen(filename, "rb");

   if(fp == NULL){

      printf("\n\nERROR: FILE COULD NOT BE OPENED\n\n");
      return NULL;

   }




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

