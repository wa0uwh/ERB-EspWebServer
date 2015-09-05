// SVG_Line_FH

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
lineGraphicFH( int w = 400, int h = 150 )
{
    long sz = 0;
    
    int Lo = FreeHeapScaleLo/100;
    int Hi = FreeHeapScaleHi/100;
    
    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();
    
    sz += wprintln(  );
    sz += wprintln(  F("<!-- Graphic -->") );
    
    // SVG Header
    sz += wprintln( sF("<svg ") );
    sz += wprintln( sF(" width='") + String(w) + F("' height='") + String(h) + F("'") );
    sz += wprintln(  F(" xmlns='http://www.w3.org/2000/svg'") );
    sz += wprintln(  F(" xmlns:xlink='http://www.w3.org/1999/xlink'") );
    sz += wprintln(  F(" version='1.1'>") );
        
    sz += wprintln(  );
    sz += wprintln(  F("<!-- Rectangle -->") );
    sz += wprint  (  F("<rect") );
    sz += wprintln( sF(" width='") + String(w) + F("' height='") + String(h) + F("'"));
    sz += wprintln(  F(" style='fill:lightblue; stroke:black; stroke-width:1' />") );
        
    sz += wprintln(  );       
    sz += wprintln(  F("<!-- Grid -->") );
    sz += wprintln(  F("<g style='fill:none; stroke:White; stroke-width:1'>") );
    {
      for (int y = Lo; y <= Hi; y += 40)
      {   
        int yg = y;
        yg = map(yg, Lo, Hi, h-10, 10);
        sz += wprintln( sF(" <path D='M 15 ") + String( yg ) + F(" H ") + String( w-10 ) + F("'/>") );
      }
    }
    sz += wprintln(  F("</g>") );
    
    sz += wprintln(  );       
    sz += wprintln(  F("<!-- Labels -->") );
    sz += wprintln(  F("<g text-anchor='end' font-size='10' style='fill:red; stroke:red; stroke-width:.1;'>") );
    {
      for (int y = Lo; y <= Hi; y += 40)
      {   
        int yg = y;
        yg = map(yg, Lo, Hi, h-10, 10);
        sz += wprintln( sF(" <text x='14' y='") + String( yg+2 ) + F("' >") + String( y/10 ) + F("</text>") );
      }
    }
    sz += wprintln(  F("</g>") );
    
    
    sz += wprintln(  );
    sz += wprintln(  F("<!-- Data -->") );
    {
      sz += wprintln(  F(" <polyline points='") );
      sz += wprint  (  F(" ") );
      int dataPoints = sizeof(gFreeHeapLog);
      int ygPrevious = 0;
      int xgPrevious = 0;
      boolean Saved = false;
      // Serial.println ( " Number of Data Points: " + String(dataPoints) );
      boolean DataSentOnThisLine = false;
      for (int x = 0; x < dataPoints; x++) {
         int xg = map(x, 0, dataPoints, 15, w-10);
         int xp = (x + gFreeHeapLogIndex) % dataPoints; // Index for Ring Buffer
         int yg = gFreeHeapLog[xp];
         yg = map(yg, Lo, Hi, h-10, 10);
         if (yg != ygPrevious) { // Avoiding Transfer of Duplicate Y Data Points
           if (Saved == true) {
             sz += wprint( String(xg) + "," + String(ygPrevious) + "," );
             DataSentOnThisLine = true;
             Saved = false;
           }
           sz += wprint( String(xg) + "," + String(yg) + "," );
           DataSentOnThisLine = true;
         } 
         else {
           Saved = true;
         }
         ygPrevious = yg;
         xgPrevious = xg;
         if (!((x+1) % 8)) {
           if (DataSentOnThisLine) {
             sz += wprintln( );
             sz += wprint( F(" ") );
             DataSentOnThisLine = false;
           }
         }
      }
      if (Saved == true) {
        sz += wprint( String(xgPrevious) + "," + String(ygPrevious) + "," ); // Repeat Last point just in case it was removed
      }
      sz += wprintln(  F("'") );
      sz += wprintln(  F(" style='fill:none; stroke:black; stroke-width:.5' />") );
    }
    
    sz += wprintln( );
    sz += wprintln(  F("</svg>"));
    
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
handleLineGraphFH()
{
    long sz = 0;
    gSentSize = 0;
      
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: image/svg+xml") );
      sz += wprintln( ); // A Blank Line
      
      sz += wprintln( F("<center>") );
      sz += lineGraphicFH();      
      sz += wprintln( F("</center>") );
      
      sz += wprint( "", true); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();

    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################
// End
