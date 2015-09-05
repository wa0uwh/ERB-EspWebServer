// B_SVG_Inst

/*
 * Copyright (c) 2015, Eldon R. Brown - ERB - WA0UWH - eldonb@ebcon.com
 *
 * See my Blog, at: http://WA0UWH.blogspot.com
 *
 * See my Source at GitHub, at: https://github.com/wa0uwh/ERB-EspWebServer
 *
 * All rights reserved. See FULL Copyright in Main or Info Page for Exclusions
 *
 * This Effort was Inspired by work by: Majenko Technologies - 2015 Esp8266 AdvancedWebServer
 *
 * See also Arduino IDE, at: https://github.com/esp8266/Arduino
 */


// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function provides the Content for the URL Requsest from WIFI
//
long
ICACHE_FLASH_ATTR
instGauge( int w = 200, int h = 200)
{
    long sz = 0;
    
    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();
    
    int startingFreeHeap = ESP.getFreeHeap();
    
    sz += wprintln( );
    sz += wprintln(  F("<!-- Create Instrument Graphic Image -->") );
    
    // SVG Header
    sz += wprintln(  F("<svg") ); 
    sz += wprintln( sF(" width='") + String(w) + F("' height='") + String(h) + F("'") );
    sz += wprintln(  F(" viewBox='-100 -100 200 200'") );
    sz += wprintln(  F(" xmlns='http://www.w3.org/2000/svg'") ); 
    sz += wprintln(  F(" xmlns:xlink='http://www.w3.org/1999/xlink'") ); 
    sz += wprintln(  F(" version='1.1'>") );
   
    
    // Rim Color Gradient
    sz += wprintln();
    sz += wprintln(  F("<!-- Rim Color Gradient -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <radialGradient id='gradRim' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln(  F("  <stop offset='0%'   style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln(  F("  <stop offset='100%' style='stop-color:Black; stop-opacity:1'/>") );
    sz += wprintln(  F(" </radialGradient>") );
    sz += wprintln(  F("</defs>") );
    
    // Face Color Gradient
    sz += wprintln();
    sz += wprintln(  F("<!-- Face Color Gradient -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <radialGradient id='gradFace' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln(  F("  <stop offset='0%'   style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln(  F("  <stop offset='100%' style='stop-color:#111122; stop-opacity:1'/>") );
    sz += wprintln(  F(" </radialGradient>") );
    sz += wprintln(  F("</defs>") );
  
    // Dish Color Gradient
    sz += wprintln();
    sz += wprintln(  F("<!-- Dish Color Gradient -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <radialGradient id='gradDish' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln(  F("  <stop offset='0%'   style='stop-color:#111122; stop-opacity:1'/>") );
    sz += wprintln(  F("  <stop offset='100%' style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln(  F(" </radialGradient>") );
    sz += wprintln(  F("</defs>") );
    
    // Bezel
    sz += wprintln();
    sz += wprintln(  F("<!-- Besel Color Gradient -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <ellipse id='instRim'") );
    sz += wprintln(  F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln(  F("   style='fill:url(#gradRim); stroke:black; stroke-width:.5'") );
    sz += wprintln(  F(" />") );
    sz += wprintln(  F("</defs>") );

    
    // Dish
    sz += wprintln();
    sz += wprintln(  F("<!-- Dish -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <ellipse id='instDish'") );
    sz += wprintln(  F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln(  F("   style='fill:url(#gradDish); stroke:black; stroke-width:3'") );
    sz += wprintln(  F(" />") );
    sz += wprintln(  F("</defs>") );
    
    // Face
    sz += wprintln();
    sz += wprintln(  F("<!-- Face -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <ellipse id='instFace'") );
    sz += wprintln(  F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln(  F("   style='fill:url(#gradFace); stroke:black; stroke-width:0'") );
    sz += wprintln(  F(" />") );
    sz += wprintln(  F("</defs>") );
        
    // Major Tic
    sz += wprintln();
    sz += wprintln(  F("<!-- Major Tic -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <polyline id='tic1' points=") );
    sz += wprintln(  F("   '0,85 ,0,100'") );
    sz += wprintln(  F("   style='stroke:white; stroke-width:3'") );
    sz += wprintln(  F("   transform='rotate(180) scale(.72)'") );
    sz += wprintln(  F(" />") );
    sz += wprintln(  F("</defs>") );
         
    // Minor Tic
    sz += wprintln();
    sz += wprintln(  F("<!-- Minor Tic-->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <polyline id='tic2' points=") );
    sz += wprintln(  F("   '0,95 ,0,100'") );
    sz += wprintln(  F("   style='stroke:white; stroke-width:1'") );
    sz += wprintln(  F("   transform='rotate(180) scale(.72)'") );
    sz += wprintln(  F(" />") );
    sz += wprintln(  F("</defs>") );
    
    // Pointer
    sz += wprintln();
    sz += wprintln(  F("<!-- Pointer -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F(" <g id='pointerMaster'>") );
    sz += wprintln(  F("   <polyline points=") );
    sz += wprintln(  F("     '0,-5 ,5,0, 2,95, 0,100, -2,95, -5,0, 0,-5, 0,60 '") );
    sz += wprintln(  F("     style='stroke:#000000; stroke-width:0.5'") );
    sz += wprintln(  F("     fill-opacity='1' stroke-opacity='0.4'") );
    sz += wprintln(  F("     transform='rotate(180) scale(.70)'") );
    sz += wprintln(  F("   />") );
    sz += wprintln(  F(" </g>") );
    sz += wprintln(  F("</defs>") );
    
    // Pointer
    sz += wprintln();
    sz += wprintln(  F("<!-- Red Pointer -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F("  <use id='pointerRed' xlink:href='#pointerMaster' style='fill:#FF4400'/>") );
    sz += wprintln(  F("</defs>") );
       
    // Pointer
    sz += wprintln();
    sz += wprintln(  F("<!-- White Pointer -->") );
    sz += wprintln(  F("<defs>") );
    sz += wprintln(  F("  <use id='pointerWhite' xlink:href='#pointerMaster' style='fill:White'/>") );
    sz += wprintln(  F("</defs>") );
      
    // Create the Image
    sz += wprintln();
    sz += wprintln(  F("<!-- Create the Image -->") );
    sz += wprintln(  F("<g text-anchor='middle'>") );
    
    sz += wprintln();
    sz += wprintln(  F("<!-- Create Dial Face -->") );
    {
        sz += wprintln(  F("  <use xlink:href='#instRim'  transform='scale(0.99)'/>") );
        sz += wprintln(  F("  <use xlink:href='#instDish' transform='scale(0.90)'/>") );
        sz += wprintln(  F("  <use xlink:href='#instFace' transform='scale(0.85)'/>") );
        
        sz += wprintln();
        sz += wprintln(  F("<!-- Create Scales and Pointers -->") );
        
        sz += wprintln(  F("<g transform='scale(0.75,1.0)'>") );
            {
              sz += wprintln();
              sz += wprintln(  F("<!-- Create VDD Scale -->") );
              const int LO_VAL = VddScaleLo - 60;
              const int HI_VAL = VddScaleHi + 60;
              const int LO_ANGLE =   (0 + 15) * 10; // Times 10 Multiplier used to increase Image resolution
              const int HI_ANGLE = (180 - 30) * 10;
              
              // Create the Tic Marks and Labels
              for (int val = LO_VAL; val <= HI_VAL; val += 20) {
                 int angle = map(val, LO_VAL, HI_VAL, HI_ANGLE, LO_ANGLE);
                 if (! (val % 100)) { // Major Tic with Label
                   sz += wprintln( sF("<g transform='rotate(") + String(angle/10.0, 1) + F(")'>") );
                   sz += wprintln(  F("  <use  xlink:href='#tic1'/>") );
                   if ((val/100) % 2) { // Is Odd
                     sz += wprintln( sF("  <text x='0' y='-75' font-size='16'>") + String(val/1000.0, 1) + F("</text>") );
                   }
                   sz += wprintln(  F("</g>") );
                 }
                 else if (! (val % 2)) { // Is Even, Minor Tic
                   sz += wprintln( sF("<g transform='rotate(") + String(angle/10.0, 1) + F(")'>") );
                   sz += wprintln(  F("  <use  xlink:href='#tic2'/>") );
                   sz += wprintln(  F("</g>") );
                 }
              }
             // Map the VDD Pointer
             int vdd = readvdd33(); // Voltage
             sz += wprintln();
             sz += wprintln(  sF("<!-- VDD Pointer, RawVDD=") + String(vdd) + F(" -->") ); // Raw Vdd Value
             vdd = constrain(vdd, LO_VAL, HI_VAL);
             vdd = map(vdd, LO_VAL, HI_VAL, HI_ANGLE, LO_ANGLE);
             sz += wprint  ( F("  <use xlink:href='#pointerRed' transform='rotate(") );
             sz += wprintln( String(vdd/10.0, 1) + F(")'/>") ); // Vdd Pointer      
            }

            sz += wprintln();
            sz += wprintln(  F("<!-- Create Free Memory Scale -->") );
            {
              const int LO_VAL = FreeHeapScaleLo - 600;
              const int HI_VAL = FreeHeapScaleHi + 600;
              const int LO_ANGLE = (180 + 30) * 10; // Times 10 Multiplier used to increase Image resolution
              const int HI_ANGLE = (360 - 15) * 10;
              
              // Create the Tic Marks and Labels
              for (int val = LO_VAL; val <= HI_VAL; val += 100) {
                 int angle = map(val, LO_VAL, HI_VAL, LO_ANGLE, HI_ANGLE);
                 if (! (val % 2000)) { // Major Tic with Label
                   sz += wprintln( sF("<g transform='rotate(") + String(angle/10.0, 1) + F(")'>") );
                   sz += wprintln(  F("  <use  xlink:href='#tic1'/>") );
                   if (! (val % 4000)) {
                     sz += wprintln( F("  <text x='0' y='-75' font-size='16'>") );
                     sz += wprintln( String(val/1000) + F("</text>") );
                   }
                   sz += wprintln(  F("</g>") );
                 }
                 else if (! (val % 500)) { // Minor Tic
                   sz += wprintln( sF("<g transform='rotate(") + String(angle/10.0, 1) + F(")'>") );
                   sz += wprintln(  F("  <use  xlink:href='#tic2'/>") );
                   sz += wprintln(  F("</g>") );
                 }
              }
             {
               // Map the Initial Free Heap Pointer 
               int fm = startingFreeHeap; // Initial Free Memory
               sz += wprintln();
               sz += wprintln(  sF("<!-- Starting FM Pointer, RawFM=") + String(fm) + F(" -->") ); // Raw FM Value
               fm  = constrain(fm, LO_VAL, HI_VAL);
               fm  = map(fm, LO_VAL, HI_VAL, LO_ANGLE, HI_ANGLE);
               sz += wprint ( F("  <use xlink:href='#pointerRed' transform='rotate(") );
               sz += wprintln( String(fm/10.0, 1)  + F(")'/>") ); // FreeMem Pointer
             }
             
             {
               // Map the Current Free Heap Pointer 
               int fm = ESP.getFreeHeap(); // Free Memory
               sz += wprintln();
               sz += wprintln(  sF("<!-- Current FM Pointer, RawFM=") + String(fm) + F(" -->") ); // Raw FM Value
               fm  = constrain(fm, LO_VAL, HI_VAL);
               fm  = map(fm, LO_VAL, HI_VAL, LO_ANGLE, HI_ANGLE);
               sz += wprint  ( F("  <use xlink:href='#pointerRed' transform='rotate(") );
               sz += wprintln( String(fm/10.0, 1)  + F(")'/>") ); // FreeMem Pointer
             }
            } 
            
        sz += wprintln(  F("</g>") );
        
        // Crystal/Glass Labels
        sz += wprintln();
        sz += wprintln(  F("<!-- Create Crystal/Glass Labels -->") );
        sz += wprintln(  F("  <text x='-30' y='-7'  font-size='10'>Free</text>") ); 
        sz += wprintln(  F("  <text x='-30' y='+4'  font-size='10'>Mem</text>") ); 
        sz += wprintln(  F("  <text x='-30' y='+14' font-size='10'>(K)</text>") );       
        sz += wprintln(  F("  <text x='+30' y='-3'  font-size='10'>VDD</text>") );       
        sz += wprintln(  F("  <text x='+30' y='+8'  font-size='10'>(Volts)</text>") );      
        sz += wprintln(  F("  <text x='+00' y='+80' font-size='08'>WA0UWH</text>") );
        
        // Center of Inst
        sz += wprintln(  F("  <use xlink:href='#instFace' transform='scale(0.12, 0.60)'/>") );
        
    }
    sz += wprintln();
    sz += wprintln(  F("</g>") );
    
    sz += wprintln(  F("</svg>") );
    
    DEBUG_MONITOR_REPORT_END();
    
    return sz;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function Handles the URL Requsest from WIFI
// and reports activity on Serial
//
void
ICACHE_FLASH_ATTR
handleInst()
{
    long sz = 0;
    gSentSize = 0;
    
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: image/svg+xml") );
      sz += wprintln( ); // A Blank Line
      
      sz += instGauge(200, 200); // Create SVG Image
      
      sz += wprintln( "", true ); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();
      
    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################    
// End
