// AdminPage

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
adminPage( int aAutoRefresh = -1 )
{
    long sz = 0;

    Serial.println ( sF("\nStart /admin Build for: ") + String(ipa2str(gServer.client().remoteIP())) +F(" . . ") );
    
    // Generate Html Header
    sz += htmlPageHeader( gDeviceName, aAutoRefresh, F("/admin") );
    
    // Page Content Starts here
    
    // Page Title Line
    sz += htmlPageTitle( gDeviceName, F("Admin") );
     
    // Navigator
    sz += navigator();
   
    // Admin's SCAN Link
    sz += wprintln( F("\r\n<!-- Options -->") );
    if (gAutoAdmin) 
         sz += wprint( F(" | <a href='/admin/auto_off'>ManUpdate</a>") );
    else sz += wprint( F(" | <a href='/admin/auto_on'>AutoUpdate</a>") );
    sz += wprint( F(" | <a href='/scanwifi'>ScanWiFi</a>") );
    sz += wprintln( F(" |") );
    

    sz += wprintln( );
    sz += wprintln( F("<!-- Page Report -->") );
    
    sz += wprintln( F("<ul>") ); // Html Indent the next Section
    {
 
      if ( gServer.uri() ==  F("/scanwifi") ) {  // Report results of AP Scan 
          sz += scanWifi();
      }
      else if ( gReStartSTN == true ) { // New Station AP Connection Message
          sz += wprintln( sF("<br>Connecting to: <b>") + String(WiFi.SSID()) + F("</b> ") );
          sz += wprintln(  F("<br>") );
          sz += wprintln(  F("<br><b>*** ReConfiguring Browser to New AP Channel, ReStarting in 10 Seconds ***</b>") );
          sz += wprintln(  F(" <ul>") );
          sz += wprintln(  F("   <form method='POST' action='/admin'>") );
          sz += wprintln(  F("   <br><ul><input type='submit' value='OK'></ul>") );
          sz += wprintln(  F("  </form>") );
          sz += wprintln(  F(" </ul>") );
      } 
      else {
          // Default Page Report
          sz += wprintln( F("\r\n<!-- Page Report -->") );
          sz += wprintln( F("<pre>") );
          {
            sz += wprintln(  F("<ul><b>Connected To AP:</b>") );
            sz += wprintln( sF("   SSID:    <b>") + String(WiFi.SSID()) + F("</b>") );
            sz += wprintln( sF("   RSSI:    <b>") + String(WiFi.RSSI()) + F("dBm</b>") );
            sz += wprintln( sF("   IPA:     <b>") + ipa2str(WiFi.localIP()) + F("</b>") );
            sz += wprintln( sF("   Gateway: <b>") + ipa2str(WiFi.gatewayIP()) + F("</b>") );
            sz += wprintln( sF("   NetMask: <b>") + ipa2str(WiFi.subnetMask()) + F("</b>") );
            sz += wprintln(  F("</ul>") );
          }
          sz += wprintln( F("</pre>") );
      }
    }
    sz += wprintln(  F("</ul>") ); // Html OutDent

    // Generate Html Footer
    sz += htmlPageFooter();
    
    Serial.println ( F(" . . Finshed /admin Build") );
    
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
handleAdminPage()
{
    long pageLength = 0;
    gSentSize = 0;
    
    gCurrentPage = ADMINPAGE;
    gApTimeout = millis() + 15 * MINs;
    
    digitalWrite ( gGrnLED, ON );
      gHits++;
      
      // HTTP Header
      wprintln(  F("HTTP/1.1 200 OK") );
      wprintln(  F("Content-Type: text/html") );
      wprintln( ); // A Blank Line
      
      pageLength += adminPage ( (gAutoAdmin ? 30 : -1) );
    
      pageLength += wprint( "", true ); // Final Packet

      PAGE_MONITOR_REPORT;
      
    digitalWrite ( gGrnLED, OFF );
    yield();
}
 

// ###########################################################
//////////////////////////////////////////////////////////////
//
void
ICACHE_FLASH_ATTR
handleScan()
{
    handleAdminPage();
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
void
ICACHE_FLASH_ATTR
handleConnect() // This may not work correctly, yet
{
  
    gReStartSTN = false;
    gHits++;
    
    if ( gServer.arg("ReStart") == "true" ) {
      gReStartSTN = true;
      
      gSsidPrev   = gSsid;
      gPasswdPrev = gPasswd;
      
      Serial.println (  F("\n\n ##############################") );
      Serial.println ( sF("Old AP: ") + gSsid + String(":") + gPasswd );
      
      gSsid =   gServer.arg( "nSsid" );
      gPasswd = gServer.arg( "nPasswd" );
      
      gStnLive = false; 
    
      Serial.println ( sF("New AP: ") \
        + gSsid + String(":") + gPasswd \
        + F(". ReStart Initiated: ") \
        + String(gReStartSTN == true ? "true" : "false") );
      handleAdminPage();
    
       Serial.println ( F("*** ReConfiguring Browser to New AP Channel, ReStarting in 5 Seconds ***") );
       delay(2000);
       //WiFi.mode(WIFI_AP);
       delay(3000);
       // WiFi.disconnect();
    }
    else handleAdminPage();

}

// ###########################################################    
// End
