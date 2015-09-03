// NotFound

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
// This Function Handles "unknown" URL Requsests from WIFI
// and reports activity on Serial
//
void
ICACHE_FLASH_ATTR
handleNotFound()
{
    long sz = 0;
    gSentSize = 0;
    
    PAGE_MONITOR_REPORT_START;
    
    digitalWrite ( gGrnLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 404") );
      sz += wprintln( F("Content-Type: text/html") );
      sz += wprintln( ); // A Blank Line
      
      sz += wprintln(  F("404 File Not Found") );
      sz += wprintln(  F("<br>") );
      
      sz += wprintln( sF("URI: ")       + gServer.uri() );
      sz += wprintln(  F("<br>") );
      
      sz += wprintln( sF("Method: ")    + String( gServer.method() == HTTP_GET ? "GET" : "POST" ) );
      sz += wprintln(  F("<br>") );
      
      sz += wprintln( sF("Arguments: ") + String( gServer.args() ) );
      sz += wprintln(  F("<br>") );
      
      for ( byte i = 0; i < gServer.args(); i++ ) {
          sz += wprintln( " " + gServer.argName ( i ) + ": " + gServer.arg ( i ) );
          sz += wprintln(  F("<br>") );
      }
      
      sz += wprint( "", true ); // Final Packet

      PAGE_MONITOR_REPORT_END;
      
      PAGE_MONITOR_REPORT_TOTAL;
      
    digitalWrite ( gGrnLED, OFF );
    
}

// ###########################################################
// End
