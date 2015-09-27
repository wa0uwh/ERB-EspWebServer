// HelpPage

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
helpPage()
{
    long sz = 0; // Sent Size
    
    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();
    
    // Parse Args
    for ( byte i = 0; i < gServer.args(); i++ ) {
       if (gServer.argName(i) == F("AutoHelpRefresh") ) gAutoHelpRefresh = constrain (gServer.arg(i).toInt(), 10, 600);
    }
 
    // Generate Html Header
    sz += htmlPageHeader( gDeviceName, gAutoHelpRefresh, F("/help") );
        
    // Page Content Starts here
    
    // Page Title Line
    sz += htmlPageTitle( gDeviceName, F("Help") );
        
//    // Page Refresh Button
//    {
//    String TmpBuf = "";
//    sz += wprintln( F("<center>") );
//    sz += wprintln( reFreshButton(TmpBuf, String("/help"));
//    sz += wprintln( F("</center>") );
//    }
 
    // Navigator
    sz += navigator();
   
    // Verbose Option
    sz += wprintln( );
    sz += wprintln(  F("<!-- Options -->") );
    if (gAutoHelpRefresh > 0)
         sz += wprint(  F(" | <a href='/help/auto_off'>ManUpdate</a>") );
    else sz += wprint(  F(" | <a href='/help/auto_on'>AutoUpdate</a>") );
    sz += wprintln(  F(" |") );
    sz += wprintln( F("<br>") );
    
    // SliderBar Graphic
    if (gAutoHelpRefresh > 0 ) {
      sz += wprintln( );
      sz += wprintln( F("<!-- SliderBar1 -->") );
      sz += sliderBar( F("AutoHelpRefresh"), F("Interval:"), 10, 600, 10, gAutoHelpRefresh, F("Sec"), F("/help") );
      sz += wprintln( F("<br>") );
    }
    sz += wprintln( F("<br>") );
    
    // Page Report
    sz += wprintln( );
    sz += wprintln(  F("<!-- Page Report -->") );;
    sz += wprintln(  F("<pre>") );
    
    // Page Links
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>Pages:</b>") );
    sz += wprintln(  F("   <a href='/home'        >URL/home</a>          = Home Dashboard (with refresh)") );
    sz += wprintln(  F("   <a href='/help'        >URL/help</a>          = This Message") );
    sz += wprintln(  F("   <a href='/admin'       >URL/admin</a>         = Admin Page") );
    sz += wprintln(  F("   <a href='/info'        >URL/info</a>          = General Information Page") );
    
    sz += wprintln(  F("   <a href='/test.svg'   target='test.svg'     >URL/test.svg</a>      = Just The Test Graphic") );
    sz += wprintln(  F("   <a href='/clock.svg'  target='clock.svg'    >URL/clock.svg</a>     = Just The Test Clock Graphic") );
    sz += wprintln(  F("   <a href='/inst.svg'   target='inst.svg'     >URL/inst.svg</a>      = Just The Test Inst Graphic") );
    sz += wprintln(  F("   <a href='/bar.svg'    target='bar.svg'      >URL/bar.svg</a>       = Just The Test Bar Graphic, Note: This is not Actually an SVG") );
    sz += wprintln(  F("   <a href='/sliderbar'  target='sliderbar'    >URL/sliderbar</a>     = Just The Test SliderBar Graphic") );
    sz += wprintln(  F("   <a href='/visitormap' target='visitormap'   >URL/visitormap</a>    = Just The Test VisitorMap Graphic") );
    sz += wprintln(  F("   <a href='/farm01.jpg' target='farm01.jpg'   >URL/farm01.jpg</a>    = Just The Test Esp Web Server Farm Image") );
    sz += wprintln(  F("</ul>") );
    
    // Modes
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>Modes:</b>") );
    sz += wprintln(  F("   <a href='/t/v'         >URL/t/v</a>           = Toggle Verbose Mode") );
    sz += wprintln( sF("   <a href='/s/v'         >URL/s/v</a>           = Verbose Mode <b>")       + String( gVerbose ? "(current)" : "") + F("</b>") );
    sz += wprintln( sF("   <a href='/c/v'         >URL/c/v</a>           = Silent Mode  <b>")       + String(!gVerbose ? "(current)" : "") + F("</b>") );
    sz += wprintln( sF("   <a href='/t/redled'    >URL/t/redled</a>      = Toggle Red Led, is <b>") + String(gRedLedState == ON ? "ON" : "OFF") + F("</b>") );
    sz += wprintln(  F("   <a href='/on/redled'   >URL/on/redled</a>     = Turn Red Led ON") );
    sz += wprintln(  F("   <a href='/off/redled'  >URL/off/redled</a>    = Turn Red Led OFF") );
    sz += wprintln(  F("</ul>") );
    
    // Querys
    sz += wprintln( );
    String TmpNodeName = String(gDeviceName) + F(".local");
    String TmpUrlNod = sF("<a href='http://") + TmpNodeName + "/'>" + TmpNodeName + "</a>";
    String TmpUrlIPA = sF("<a href='http://") + ipa2str(WiFi.localIP()) + "/'>" + ipa2str(WiFi.localIP()) + "</a>";
    
    sz += wprintln(  F("<ul><b>Query:</b>") );
    sz += wprintln( sF("   <a href='/q/ssid' target='query'      >URL/q/ssid</a>        = SSID:      <b>")         + String(WiFi.SSID()) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/rssi' target='query'      >URL/q/rssi</a>        = RSSI:      <b>")         + String(WiFi.RSSI()) + F("dBm</b>") );
    sz += wprintln( sF("   <a href='/q/mdns' target='query'      >URL/q/mdns</a>        = mDNS Name: <b>")         + String(TmpUrlNod) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/stnipa' target='query'    >URL/q/stnipa</a>      = STN IPA:   <b>")         + String(TmpUrlIPA) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/stnmac' target='query'    >URL/q/stnmac</a>      = STN Mac:   <b>")         + String(mac2str(WiFi.macAddress(gMacBuf))) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/gateway' target='query'   >URL/q/gateway</a>     = Gateway:   <b>")         + String(ipa2str(WiFi.gatewayIP())) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/netmask' target='query'   >URL/q/netmask</a>     = Netmask:   <b>")         + String(ipa2str(WiFi.subnetMask())) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/myipa' target='query'     >URL/q/myipa</a>       = My IPA:    <b>")         + String(ipa2str(gServer.client().remoteIP())) + F("</b>") );
//  sz += wprintln( sF("   <a href='/q/myport' target='query'    >URL/q/myport</a>      = My Port:   <b>")         + String(gServer.client().remotePort()) + F("</b>") );
    
    sz += wprintln( );
    sz += wprintln( sF("   <a href='/q/restarts' target='query'  >URL/q/restarts</a>    = WiFi ReStarts:     <b>") + String(gWifiReStarts) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/redled' target='query'    >URL/q/redled</a>      = Red Led State:     <b>") + String(gRedLedState == ON ? "ON" : "OFF") + F("</b>") );
    sz += wprintln( sF("   <a href='/q/uptime' target='query'    >URL/q/uptime</a>      = Uptime Seconds:    <b>") + String(upTime()) + F("s</b>") );
//  sz += wprintln( sF("   <a href='/q/utctime' target='query'   >URL/q/utctime</a>     = Utctime Seconds:   <b>") + String(utcTime()) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/epoch' target='query'     >URL/q/epoch</a>       = Epoch (Unix Time): <b>") + String(epoch()) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/loadavg' target='query'   >URL/q/loadavg</a>     = Load Avg (Pseudo): <b>") + String(gLoadAvgS) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/battvolt' target='query'  >URL/q/battvolt</a>    = Battery Voltage:   <b>") + String(readvdd33()/1000.0, 2) + F("V</b>") );
    sz += wprintln( sF("   <a href='/q/freeheap' target='query'  >URL/q/freeheap</a>    = Free Heap Size:    <b>") + String(ESP.getFreeHeap() / 1000.0, 3) + F("KB</b>") );
    sz += wprintln( sF("   <a href='/q/hits' target='query'      >URL/q/hits</a>        = Hit Counter:       <b>") + String(gHits) + F("</b>") );
    
    sz += wprintln( sF("   <a href='/q/id' target='query'        >URL/q/id</a>          = Unit ID:           <b>") + String(ESP.getChipId() / 10000.0, 4) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/idx' target='query'       >URL/q/idx</a>         = Unit ID Hex:       <b>") + String(id2hex(ESP.getChipId())) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/flashid' target='query'   >URL/q/flashid</a>     = Flash ID:          <b>") + String(ESP.getFlashChipId() / 10000.0, 4) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/flashsize' target='query' >URL/q/flashsize</a>   = Flash Size:        <b>") + String(ESP.getFlashChipSize() / 1000.0, 1) + F("KB</b>") );
    sz += wprintln( sF("   <a href='/q/flashspd' target='query'  >URL/q/flashspd</a>    = Flash Speed:       <b>") + String(ESP.getFlashChipSpeed() / 1000000.0, 1) + F("MHz</b>") );
    
    sz += wprintln( sF("   <a href='/q/rev' target='query'       >URL/q/rev</a>         = Software Revision: <b>") + String(gRev) + F("</b>") );
    sz += wprintln(  F("</ul>") );
    
    // AP Mode Querys
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>AP Mode Query:</b>") );
    sz += wprintln( sF("   <a href='/q/apssid' target='query'    >URL/q/apssid</a>      = AP SSID:           <b>") + String(gDeviceName) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/apipa ' target='query'    >URL/q/apipa</a>       = AP IPA:            <b>") + String(ipa2str(WiFi.softAPIP())) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/apmac' target='query'     >URL/q/apmac</a>       = AP Mac:            <b>") + String(mac2str(WiFi.softAPmacAddress(gMacBuf))) + F("</b>") );
    sz += wprintln( sF("   <a href='/q/apchannel' target='query' >URL/q/apchannel</a>   = AP Only Channel:   <b>") + String(gApChannel) + F("</b>") );
    sz += wprintln(  F("</ul>") );
    
    // Information
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>Information:</b>") );
    sz += wprintln(  F("   <a href='/help'        >URL/help</a>          = This Message") );
    sz += wprintln(  F("   <a href='/info'        >URL/info</a>          = General Information and Copyright Message") );
    sz += wprintln(  F("</ul>") );
    
    // Admin
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>Admin:</b>") );
    sz += wprintln(  F("   <a href='/admin'       >URL/admin</a>         = Admin Page") );
    sz += wprintln(  F("   <a href='/scanwifi'    >URL/scanwifi</a>      = Admin WIFI Scan") );
    sz += wprintln(  F("</ul>") );
     
    // Error Handler
    sz += wprintln( );
    sz += wprintln(  F("<ul><b>Errors:</b>") );  
    sz += wprintln(  F("   <a href='/else'        >URL/else</a>          = All else = Error Message") );
    sz += wprintln(  F("</ul>") );
    
    sz += wprintln(  F("</pre>") );

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
handleHelpPage()
{
    int sz = 0;
    gSentSize = 0;
  
    gCurrentPage = HELPPAGE;
    
    digitalWrite ( gGrnLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: text/html") );
      sz += wprintln( ); // A Blank Line
      
      sz += helpPage();
      
      sz += wprint( "", SEND_FINISH ); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();
  
    digitalWrite ( gGrnLED, OFF );
    yield();
}

// ###########################################################
// End
