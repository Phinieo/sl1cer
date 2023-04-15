
//RETURNS THE LENGTH OF FILAMENT NEEDED TO PRODUCE AN PERIMETER EXTRUSION OF THE INPUT LENGTH
float perimeterExtrusion(float extrudeDist){

   //ASSUMES RECTANGULAR CROSS SECTION OF EXTRUSION
   float extrudeVolume = extrudeDist * (LAYER_HEIGHT * EXTRUSION_WIDTH);
   
   float extrudeFlow = extrudeVolume / (3.141565 * (FILAMENT_DIAMETER/2) * (FILAMENT_DIAMETER/2));


   return extrudeFlow;

}




//RETURNS THE LENGTH OF FILAMENT NEEDED TO PRODUCE AN PERIMETER EXTRUSION OF THE INPUT LENGTH
float infillExtrusion(float extrudeDist){

   //ASSUMES RECTANGULAR CROSS SECTION OF EXTRUSION
   float extrudeVolume = extrudeDist * (LAYER_HEIGHT * INFILL_WIDTH);
   
   float extrudeFlow = extrudeVolume / (3.141565 * (FILAMENT_DIAMETER/2) * (FILAMENT_DIAMETER/2));


   return extrudeFlow;

}
