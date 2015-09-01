// B_SVG_Line

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
lineGraphic( int w = 300, int h = 100 )
{
    long sz = 0;
    
    Serial.println ( sF("\nStart Line Graph Build for: ") + String(ipa2str(gServer.client().remoteIP())) +F(" . . ") );
    
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
    sz += wprintln(  F("<rect") );
    sz += wprint  ( sF(" width='") + String(w) + F("' height='") + String(h) + F("'") );
    sz += wprintln(  F(" style='fill:lightblue; stroke:black; stroke-width:1' />") );
        
    sz += wprintln(  );       
    sz += wprintln(  F("<!-- Data -->") );
    sz += wprintln(  F(" <polyline points='") );
    int y = random(h-20);
    for (int x = 0, y = h/2; x < 100; x++) {
       int xg = map(x, 0, 99, 10, w-10);
       y += random(-20, 20);
       int yg = map(y, 0, 100, h-10, 10);
       sz += wprint( String(xg) + "," + String(yg) + "," );
       if (!((x+1) % 8)) {
         sz += wprintln( );
       }
    }
    
    sz += wprintln( );
    sz += wprintln(  F("' style='fill:none; stroke:black; stroke-width:1' />") );

    sz += wprintln(  F("</svg>") );
    
    Serial.println ( F(" . . Finshed Line Graph Build") );
    yield();
    
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
handleLineGraph()
{
    long pageLength = 0;
    gSentSize = 0;
    
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      // HTTP Header
      wprintln( F("HTTP/1.1 200 OK") );
      wprintln( F("Content-Type: image/svg+xml") );
      wprintln( ); // A Blank Line
      
      pageLength += lineGraphic();
      
      pageLength += wprint( "", true ); // Final Packet

      PAGE_MONITOR_REPORT;
      
    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################
// End
