// B_SVG_Clock

  /*
   * Copyright (c) 2015, Eldon R. Brown - ERB - WA0UWH - eldonb@ebcon.com
   *
   * See Blog at: http://WA0UWH.blogspot.com
   *
   * All rights reserved. See FULL Copyright in Main.h or Info Page for Exclusions
   *
   * Inspired by work by: Majenko Technologies - 2015 Esp8266 AdvancedWebServer
   *
   * See: https://github.com/esp8266/Arduino
   */ 
 

// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function provides the Content for the URL Requsest from WIFI
//
long
ICACHE_FLASH_ATTR
clockGauge(int w = 200, int h = 200)
{
    long sz = 0;

    PAGE_MONITOR_REPORT_START;
    
    sz += wprintln( );
    sz += wprintln( F("<!-- Dial Graphic Image -->") );
    
    // SVG Header
    sz += wprintln(  F("<svg") );
    sz += wprintln( sF(" width='") + String(w) + F("' height='") + String(h) + F("'") );
    sz += wprintln(  F(" viewBox='-100 -100 200 200'") );
    sz += wprintln(  F(" xmlns='http://www.w3.org/2000/svg'") ); 
    sz += wprintln(  F(" xmlns:xlink='http://www.w3.org/1999/xlink'") ); 
    sz += wprintln(  F(" version='1.1'>") );
   
    // Red LED Color Gradient
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <radialGradient id='RedLED' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln( F("  <stop offset='0%'   style='stop-color:#FF4400; stop-opacity:1'/>") );
    sz += wprintln( F("  <stop offset='100%' style='stop-color:Red; stop-opacity:1'/>") );
    sz += wprintln( F(" </radialGradient>") );
    sz += wprintln( F("</defs>") );
    
    // Green LED Color Gradient
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <radialGradient id='GrnLED' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln( F("  <stop offset='0%'   style='stop-color:#44FF00; stop-opacity:1'/>") );
    sz += wprintln( F("  <stop offset='100%' style='stop-color:Green; stop-opacity:1'/>") );
    sz += wprintln( F(" </radialGradient>") );
    sz += wprintln( F("</defs>") );   
    
    // Rim Color Gradient
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <radialGradient id='gradRim' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln( F("  <stop offset='0%'   style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln( F("  <stop offset='100%' style='stop-color:Black; stop-opacity:1'/>") );
    sz += wprintln( F(" </radialGradient>") );
    sz += wprintln( F("</defs>") );
    
    // Face Color Gradient
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <radialGradient id='gradFace' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln( F("  <stop offset='0%'   style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln( F("  <stop offset='100%' style='stop-color:#111122; stop-opacity:1'/>") );
    sz += wprintln( F(" </radialGradient>") );
    sz += wprintln( F("</defs>") );
  
    // Dish Color Gradient
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <radialGradient id='gradDish' cx='50%' cy='50%' r='90%' fx='30%' fy='30%'>") );
    sz += wprintln( F("  <stop offset='0%'   style='stop-color:#111122; stop-opacity:1'/>") );
    sz += wprintln( F("  <stop offset='100%' style='stop-color:#DDDDDD; stop-opacity:1'/>") );
    sz += wprintln( F(" </radialGradient>") );
    sz += wprintln( F("</defs>") );
    
    // Bezel
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <ellipse id='dialRim'") );
    sz += wprintln( F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln( F("   style='fill:url(#gradRim); stroke:black; stroke-width:.5'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
    
    // Red LED
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <ellipse id='RedLed'") );
    sz += wprintln( F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln( F("   style='fill:url(#RedLED); stroke:black; stroke-width:0'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
     
    // Green LED
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <ellipse id='GrnLed'") );
    sz += wprintln( F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln( F("   style='fill:url(#GrnLED); stroke:black; stroke-width:0'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
    
    // Dish
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <ellipse id='dialDish'") );
    sz += wprintln( F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln( F("   style='fill:url(#gradDish); stroke:black; stroke-width:3'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
    
    // Face
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <ellipse id='dialFace'") );
    sz += wprintln( F("   cx='0' cy='0' rx='100' ry='100'") );
    sz += wprintln( F("   style='fill:url(#gradFace); stroke:black; stroke-width:0'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
        
    // Major Tic
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <polyline id='tic1' points=") );
    sz += wprintln( F("   '0,85 ,0,100'") );
    sz += wprintln( F("   style='stroke:white; stroke-width:3'") );
    sz += wprintln( F("   transform='rotate(180) scale(.70)'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
         
    // Minor Tic
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <polyline id='tic2' points=") );
    sz += wprintln( F("   '0,95 ,0,100'") );
    sz += wprintln( F("   style='stroke:white; stroke-width:1'") );
    sz += wprintln( F("   transform='rotate(180) scale(.70)'") );
    sz += wprintln( F(" />") );
    sz += wprintln( F("</defs>") );
    
    // Pointer
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F(" <g id='pointerMaster'>") );
    sz += wprintln( F("   <polyline points=") );
    sz += wprintln( F("     '0,-5 ,5,0, 2,95, 0,100, -2,95, -5,0, 0,-5, 0,60 '") );
    sz += wprintln( F("     style='stroke:#000000; stroke-width:0.5'") );
    sz += wprintln( F("     fill-opacity='1' stroke-opacity='0.4'") );
    sz += wprintln( F("     transform='rotate(180) scale(.68)'") );
    sz += wprintln( F("   />") );
    sz += wprintln( F(" </g>") );
    sz += wprintln( F("</defs>") );
    
    // Pointer
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F("  <use id='pointerRed' xlink:href='#pointerMaster' style='fill:#FF4400'/>") );
    sz += wprintln( F("</defs>") );
       
    // Pointer
    sz += wprintln( );
    sz += wprintln( F("<defs>") );
    sz += wprintln( F("  <use id='pointerWhite' xlink:href='#pointerMaster' style='fill:White'/>") );
    sz += wprintln( F("</defs>") );
      
    // Create the Image 
    sz += wprintln( );
    sz += wprintln( F("<!-- Create The Image -->") );
    sz += wprintln( F("<g text-anchor='middle'>") );
    {
        sz += wprintln( F("  <use xlink:href='#dialRim'     transform='scale(0.99)'/>") );
        sz += wprintln( F("  <use xlink:href='#dialDish'    transform='scale(0.90)'/>") );
        sz += wprintln( F("  <use xlink:href='#dialFace'    transform='scale(0.85)'/>") );
        
        // Create the Tic Marks and Labels
        sz += wprintln( );
        sz += wprintln( F("  <!-- Tic Marks and Labels -->") );
        for (int a = 1; a <= 120; a += 1) {
               {
                   if (! (a % 5)) {
                         sz += wprint( sF("  <g transform='rotate(") + String(3*a) + F(")'>") );
                         // 24 Hours 
                         sz += wprint( sF("<text x='0' y='-45' font-size='6'>") + String(a/5) + F("</text>") );
                       
                         if (! (a % 10)) {
                           // Major Tic
                           sz += wprint(  F("<use  xlink:href='#tic1'/>") );
                           // Hours 
                           sz += wprint( sF("<text x='0' y='-72' font-size='16'>") + String(a/5/2) + F("</text>") );     
                           // Minutes
                           sz += wprint( sF("<text x='0' y='-52' font-size='8' fill='red'>") + String(a/2) + F("</text>") );
                         } 
                         sz += wprintln( F("</g>") );
                   }
                   else if (! (a % 2)) {
                     sz += wprint( sF("  <g transform='rotate(") + String(3*a) + F(")'>") );
                     // Minor Tic
                     sz += wprint( F("<use  xlink:href='#tic2'/>") );
                     sz += wprintln( F("</g>") );
                   }
               }
        }

      
        // LED and Pointers
        sz += wprintln( );
        sz += wprintln( F("  <!-- LED and Pointer -->") );
        if (gEpochAvailable == false) {
          sz += wprintln( F("  <use xlink:href='#RedLed' transform='translate(0,38) scale(.03)'/>") );
        }
        else {
          sz += wprintln( F("  <use xlink:href='#GrnLed' transform='translate(0,38) scale(.03)'/>") );
        
          // Create the Pointers
          {
            long Secs = epoch();
            long Mins = Secs / 60;
            long Hrs =  Mins / 60;
            Secs %= 60;
            Mins %= 60;
            Hrs  %= 24;
            
            sz += wprint  ( F("  <use xlink:href='#pointerRed'   transform='rotate(") );
            sz += wprintln( String( ((Hrs - 7 + Mins/60.0) * 30) ) + F(") scale(1.5,0.75)'/>") ); // Hour Hand, PDT
            sz += wprint  ( F("  <use xlink:href='#pointerWhite' transform='rotate(") );
            sz += wprintln( String( ((Hrs + 0 + Mins/60.0) * 15) ) + F(") scale(1.0,0.6)'/>") );  // GMT Hour Hand
            sz += wprint  ( F("  <use xlink:href='#pointerRed'   transform='rotate(") );
            sz += wprintln( String( ((Mins + Secs/60.0) * 6) ) + F(")'/>") ); // Min Hand
          }
        }
        
        // Crystal/Glass Labels
        sz += wprintln( );
        sz += wprintln( F("  <!-- Crystal/Glass Labels -->") );
        sz += wprintln( F("  <text x='0' y='-20' font-size='10'>WA0UWH</text>") );      
        sz += wprintln( F("  <text x='0' y='+30' font-size='10'>PDT / UTC</text>") );
        
        // Center of Dial
        sz += wprintln( );
        sz += wprintln( F("  <!-- Center of Dial -->") );
        sz += wprintln( F("  <use xlink:href='#dialFace' transform='rotate(30) scale(.12)'/>") );
        
        
    }
    sz += wprintln( F("</g>") );
    
    sz += wprintln( F("</svg>"));
    
    PAGE_MONITOR_REPORT_END;
    
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
handleClock() 
{
    long sz = 0;
    gSentSize = 0;
    
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: image/svg+xml") );
      sz += wprintln( ); // A Blank Line
      
      sz += clockGauge( 200, 200 );
      
      sz += wprint( "", true); // Final Packet

      PAGE_MONITOR_REPORT_TOTAL;

    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################    
// End
