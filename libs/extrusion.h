

float perimeterExtrusion(float extrudeDist){

   //ASSUMES RECTANGULAR CROSS SECTION OF EXTRUSION
   float extrudeVolume = extrudeDist * (LAYER_HEIGHT * EXTRUSION_WIDTH);
   
   float extrudeFlow = extrudeVolume / (3.141565 * (FILAMENT_DIAMETER/2) * (FILAMENT_DIAMETER/2));


   return extrudeFlow;

}
