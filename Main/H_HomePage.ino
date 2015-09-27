// HomePage

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
homePage()
{
    long sz = 0; // Sent Size

    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();

    // Parse Args
    for ( byte i = 0; i < gServer.args(); i++ ) {
       if (gServer.argName(i) == F("AutoHomeRefresh") ) gAutoHomeRefresh = constrain (gServer.arg(i).toInt(), 10, 600);
    }
    
    // Generate Html Header
    sz += htmlPageHeader( gDeviceName, gAutoHomeRefresh, gServer.uri() );
        
    // Page Content Starts here
    
    // Page Title Line
    sz += htmlPageTitle( gDeviceName, F("Dashboard") );
        
//    // Page Refresh Button
//    {
//    String TmpBuf = "";
//    sz += wprintln( F("<center>") );
//    sz += wprintln( reFreshButton(TmpBuf, String("/home")) );
//    sz += wprintln( F("</center>") );
//    }
    
    // Navigator
    sz += navigator();
    
    // Options
    sz += wprintln( );
    sz += wprintln( F("<!-- Options -->") );
    if (gAutoHomeRefresh>0)
         sz += wprint  ( F(" | <a href='/home/auto_off'>ManUpdate</a>") );
    else sz += wprint  ( F(" | <a href='/home/auto_on'>AutoUpdate</a>") );
    sz += wprint  ( F(" | <a href='/t/v'>Verbose</a>") );
    sz += wprintln( F(" |") );
    sz += wprintln( F("<br>") );

    // SliderBar Graphic
    if (gAutoHomeRefresh > 0 ) {
      sz += wprintln( );
      sz += wprintln( F("<!-- SliderBar1 -->") );
      sz += sliderBar( F("AutoHomeRefresh"), F("Interval:"), 10, 600, 10, gAutoHomeRefresh, F("Sec"), F("/home") );
      sz += wprintln( F("<br>") );
    }
    sz += wprintln( F("<br>") );


   
    sz += wprintln( );
    sz += wprintln(  F("<!-- Page Report -->") );
    
    // Report LED State, and Provide Toggle Option
    sz += wprintln( );
    sz += wprintln( F("<!-- LED Control -->") );
    sz += wprint  ( F("<form method='GET' ") );
    {
        sz += wprintln( sF("action='/") + String((gRedLedState == ON) ? "off" : "on") + F("/redled'>") );
        sz += wprintln(  F("<center>") );
        sz += wprint  ( sF("Red LED is <b>") + String((gRedLedState == ON) ? "ON" : "OFF") + F("</b> -&gt; ") );
        sz += wprintln( sF("<input type='submit' name='' value='") + String((gRedLedState == ON) ? "OFF" : "ON") + F("' />") );
        sz += wprintln(  F("</center>") );
    }
    sz += wprintln( F("</form>") );
    sz += wprintln( F("<br>") );

//    // An Experiment with Buttons
//    sz += wprintln( "<br>\r\n";
//    sz += wprintln( "<form>\r\n";
//    sz += wprintln( "<input Type=\"BUTTON\" Value=\"Off\" Onclick=\"window.location.href='/'\">\r\n";
//    sz += wprintln( "</form>\r\n";

    
    // SVG_Line Graphic
    {
      sz += wprintln( );
      sz += wprintln( F("<!-- Ramdom Line Graphic -->") );
      sz += wprintln( F("<center>") );
      {
          sz += wprintln( F("<a href='/test.svg' target='test.svg'>") );
          sz += lineGraphic();
          sz += wprintln( F("</a>") );
          
          sz += wprintln( );
          sz += wprintln( F("<br>") );
          sz += wprintln( F("<b>Example: Random") );
          sz += wprintln( F("<a href='/test.svg' target='test.svg'>Line</a> Graphic</b>") );
      }
      sz += wprintln( F("</center>") );
      sz += wprintln( F("<br>") );
    }
    
   
        
    // SVG_Bar Graphic
    sz += wprintln( );
    sz += wprintln( F("<!-- Bar Graphic -->") );
    sz += wprintln( F("<center>") );
    {
        sz += barGraphic();
        
        sz += wprintln( );
        sz += wprintln( F("<br>") );
        sz += wprintln( F("<b>Example:") );
        sz += wprintln( F("<a href='/bar.svg' target='bar.svg'>Bar</a> Graphic</b>") );
    }
    sz += wprintln( F("</center>") );
    sz += wprintln( F("<br>") );
    
    
    // SVG_Line_FH Graphic
    sz += wprintln( );
    sz += wprintln( F("<!-- Free Heap Graphic -->") );
    sz += wprintln( F("<center>") );
    {
        sz += wprintln(  F("<a href='/freeheap.svg' target='freeheap.svg'>") );
        sz += lineGraphicFH();
        sz += wprintln(  F("</a>") );
        
        sz += wprintln( );
        sz += wprintln(  F("<br>") );
        sz += wprintln(  F("<b>Example:") );
        sz += wprintln(  F("<a href='/freeheap.svg' target='freeheap.svg'>Line</a> FreeHeap Graphic") );
        sz += wprintln(  F("</b>") );
        sz += wprintln(  F("<br>") );
        sz += wprintln( sF("Range: ") + String( FreeHeapScaleLo/1000 ) + F("KB to ") + String( FreeHeapScaleHi/1000 ) + F("KB") );
        sz += wprintln(  F("<br>") );
        sz += wprintln( sF("Count of Last Sample: ") + String( gFreeHeapLogIndex/1000.0, 2 ) + F("K") );
        sz += wprintln(  F("<br>") );
        sz += wprintln(  F("Down Tick marks Start of Web Page Link, Up Tick marks End of Page") );
        sz += wprintln(  F("<br>") );
        sz += wprintln(  F("Horizontal Sustained Level, is Normal Idle") );
    }
    sz += wprintln( F("</center>") );
    sz += wprintln( F("<br>") );
    
    
    // SVG Dial Graphic
    sz += wprintln( );
    sz += wprintln( F("<!-- Dial Graphic -->") );
    sz += wprintln( F("<center>") );
    {
        // SVG_Inst Graphic
        sz += wprintln( F("<!-- Inst Graphic -->") );
        sz += wprintln( F("<a href='/inst.svg' target='inst.svg'><img src='/inst.svg' width='200' hight='200' alt='Buffering'/></a>") );
        
        // SVG_Clock Graphic
        sz += wprintln( F("<!-- Clock Graphic -->") );
        sz += wprintln( F("<a href='/clock.svg' target='clock.svg'><img src='/clock.svg' width='200' hight='200' alt='Buffering'/></a>") );
        
        sz += wprintln( );
        sz += wprintln( F("<br>") );
        sz += wprintln( F("<b>Example:") );
        sz += wprintln( F("<a href='/inst.svg' target='inst.svg'>Inst</a> and <a href='/clock.svg' target='clock.svg'>Clock</a> Graphics</b>") );

        // Provide Optional Visitor Map
        sz += visitorMap();
    } 
    sz += wprintln( F("</center>") );
    sz += wprintln( F("<br>") );
    
    // Verbose Status Info
    if (gVerbose == true ) {
        sz += wprintln( );
        sz += wprintln(  F("<!-- Verbose -->") );
        sz += wprintln(  F("<ul>") );
        sz += wprintln(  F("<b>Verbose List:</b>") );
        sz += wprintln(  F("  <ul>") );
        sz += wprintln( sF("    Uptime: <b>")          + String(upTimeStr()) + F("</b><br>") );
        sz += wprintln( sF("    Batt Voltage: <b>")    + String(readvdd33()/1000.0, 2) + F("V</b><br>") );
        sz += wprintln( sF("    Free Heap Size: <b>")  + String(ESP.getFreeHeap() / 1000.0, 3) + F("KB</b><br>") );
        sz += wprintln( sF("    Hits: <b>")            + String(gHits) + F("</b><br>") );
        sz += wprintln( sF("    Unit ID: <b>")         + String(ESP.getChipId() / 10000.0, 4) + F("</b><br>") );
        sz += wprintln( sF("    STN IPA: <b>")         + String(ipa2str(WiFi.localIP())) + F("</b><br>") );
        sz += wprintln( sF("    My IPA: <b>")          + String(ipa2str(gServer.client().remoteIP())) + F("</b><br>") );
        sz += wprintln( sF("    Sketch Rev: <b>")      + String(gRev) + F("</b><br>") );
        sz += wprintln(  F("  </ul>") );
        sz += wprintln(  F("</ul>") );
        sz += wprintln(  F("<br>") );
    }

    // Generate Html Footer
    sz += htmlPageFooter();

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
handleHomePage()
{
    long sz = 0; // Sent Size
    gSentSize = 0;
    
    gCurrentPage = HOMEPAGE;
    
    digitalWrite ( gGrnLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: text/html") );
      sz += wprintln( ); // A Blank Line
      
      sz += homePage();
    
      sz += wprint( "", SEND_FINISH ); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();
     
    digitalWrite ( gGrnLED, OFF );
    yield(); 
}

// ###########################################################
// End
