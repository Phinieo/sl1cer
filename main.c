

#include "configs.h"
#include "gcode.h"




int main(){



    float currentZ = 0.0f;
    float currentE = 0.0f;



    FILE *fp;

    fp = fopen(GCODE_OUT, "w");



    //WRITE STARTING GCODES INCLUDING HOMING AND TEMPERATURES
    //N
    writeStart(fp);

    fputs("\n\n",fp);



    //COMPUTE AND MOVE TO STARTING POSITION OF FIRST LINE
    moveToPerimeterStart(fp,2);

    fputs("\n\n",fp);



    //UNDO EARLY EXTRUDER RETRACTION
    currentE += 2.0f;

    fputs("G1 E2.00000 F2400.00000",fp);

    fputs("\n\n",fp);





    printAllPerimeters(fp,&currentZ,&currentE);




    printf("%f\n",currentZ);

    fclose(fp);

}
