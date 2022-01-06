#include <ctype.h>



struct coord{

    float X;
    float Y;
    float Z;

};

struct triangle{

    struct coord normal;
    struct coord vertex1;
    struct coord vertex2;
    struct coord vertex3;

};



struct triangle* parseSTL(char* filename){

   
    FILE *fp;

    fp = fopen(filename, "r");


    char* buffer = calloc(200, sizeof(char));


    char * stringSegment;


    while(fgets(buffer,200,fp)){


        stringSegment = strtok(buffer," ");


        while(stringSegment[0] != '-' || isdigit(stringSegment[0]) == 0){

            stringSegment = strtok(NULL," ");

        }
  

    }



    free(buffer);
 

}

