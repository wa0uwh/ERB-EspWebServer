// B_SliderBar

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
sliderBar(  String aId = "Id0", String aLabel = "Label0", int aMin = 0, int aMax = 100, int aStp = 5, int aVal = 50, String aUnits = "", String aBaseURI = "/")
{
    long sz = 0;
    
    PAGE_MONITOR_REPORT_START;
    
    PAGE_MONITOR_REPORT_ARGS;
    
    sz += wprintln(  );
    sz += wprintln(  F("<!-- SliderBar Script -->") );

    sz += wprintln(  F("<script type='text/javascript'>") );
    sz += wprintln( sF("  function ") + String( aId ) + F("_ShowValue(num) {") );
    sz += wprintln( sF("    document.querySelector('#") + String( aId ) + F("_display').value = num;") );
    sz += wprintln( sF("    document.querySelector('#") + String( aId ) + F("_action').value = num;") );
    sz += wprintln(  F("  }") );
    
    sz += wprintln(  F("</script>") );

    
    sz += wprintln(  );
    sz += wprintln(  F("<!-- SliderBar Input -->") );
    
//    sz += wprintln(  F("<form action='/home' method='get'>") );
    sz += wprintln(  sF("<form action='") + String( aBaseURI ) + F("' method='get'>") );
    {
        sz += wprintln( String( aLabel ) );
        sz += wprintln( sF("<input type='range' name='") + String( aId ) + F("'") );
        sz += wprintln( sF("  min='")  + String( aMin ) + F("' max='")   + String( aMax ) + F("'") );
        sz += wprintln( sF("  step='") + String( aStp ) + F("' value='") + String( aVal ) + F("'") );
        sz += wprintln( sF("  oninput='")  + String( aId ) + F("_ShowValue(value)'") );
        sz += wprintln(  F("/>") );
        
        sz += wprint  ( sF("<output for='") + String( aId ) + F("'") );
        sz += wprint  ( sF("  id='") + String( aId ) + F("_display'>") + String( aVal ) );
        sz += wprintln( sF("</output>") + String( aUnits ) );
        
        sz += wprintln( sF("<input type='submit' value='GO'>") );
        
//        sz += wprintln(  );
//        sz += wprint  ( sF("<output for='") + String( aId ) + F("'") );
//        sz += wprint  ( sF("  id='") + String( aId ) + F("_action'>") + String( aVal ) );
//        sz += wprintln( sF("</output>"));
     
//        sz += wprintln(  F("<br>(Not Working Yet)") );
    }
    sz += wprintln( sF("</form>") );
    
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
handleSliderBar()
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
      sz += sliderBar( F("IdData0"), F("Percent:"), 10, 90, 1, 50, F("%") );
      sz += wprintln( F("</center>") );
      
      sz += wprint( "", true ); // Final Packet

      PAGE_MONITOR_REPORT_TOTAL;
      
    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################
// End
