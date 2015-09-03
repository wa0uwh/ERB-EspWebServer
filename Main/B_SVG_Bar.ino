// B_SVG_Bar

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
barGraphic()
{
    long sz = 0;
    
    PAGE_MONITOR_REPORT_START;
        
    // HTML5 Graphic
    sz += wprintln( );
    sz += wprintln( F("<!-- Meter Bar Graphic -->") );
    
    {
      sz += wprintln( );
      sz += wprintln( F("<!-- Vdd Scale -->") );
      int Lo = VddScaleLo;
      int Hi = VddScaleHi;
      int Vdd = readvdd33();
      
      sz += wprintln( F("<br>") );
      sz += wprintln( String( Lo/1000.0, 1 )  + F("v") );
      sz += wprint  ( sF("<meter value='")+ String( Vdd ) );
      sz += wprint  ( sF("' min='") + String( Lo ) );
      sz += wprint  ( sF("' low='") + String( 3000) );
      sz += wprint  ( sF("' optium='") + String(  3300 ) );
      sz += wprint  ( sF("' high='") + String( Hi ) );
      sz += wprint  ( sF("' max='") + String( Hi ) );
      sz += wprintln( sF("'>VDD</meter>") );
      sz += wprintln( String( Hi/1000.0, 1 ) + F("v") );
      sz += wprintln(  F("<br>") );
      sz += wprintln( sF("VDD = ") + String(Vdd/1000.0, 2) + F("v") );
    }
   
    {
      sz += wprintln( );
      sz += wprintln( F("<!-- Free Heap Scale -->") );
      int Lo = FreeHeapScaleLo;
      int Hi = FreeHeapScaleHi;
      int FreeHeap = ESP.getFreeHeap();
      
      sz += wprintln( F("<br>") );
      sz += wprintln( String( Lo/1000 ) + F("KB") );
      sz += wprint  ( sF("<meter value='") + String( FreeHeap ) );
      sz += wprint  ( sF("' min='") + String( Lo ) );
      sz += wprint  ( sF("' low='") + String( Lo + 1000) );
      sz += wprint  ( sF("' optium='") + String( Lo + 2000) );
      sz += wprint  ( sF("' high='") + String( Hi ) );
      sz += wprint  ( sF("' max='") + String( Hi ) );
      sz += wprintln( sF("'>FreeMem</meter>") );
      sz += wprintln( String( Hi/1000 ) + F("KB") );
      sz += wprintln(  F("<br>") );
      sz += wprintln( sF("Free Memory = ") + String(FreeHeap/1000.0, 1) + F("KB") );
    }
    
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
handleBar()
{
    
    long sz = 0;
    gSentSize = 0;
    
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: text/html") );
      sz += wprintln( ); // A Blank Line
            
      sz += wprintln( F("<center>") );
      sz = barGraphic();      
      sz += wprintln( F("</center>") );
      
      sz += wprint( "", true ); // Final Packet

      PAGE_MONITOR_REPORT_TOTAL;

    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################
// End
