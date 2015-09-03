// ScanWiFi

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
scanWifi()
{
    long sz = 0;
    
    PAGE_MONITOR_REPORT_START;
    
    // WiFi.scanNetworks will return the number of networks found
    byte n = WiFi.scanNetworks();
    if (n == 0)
      sz += wprintln(  F(" <br>No Networks Found") );
    else
    {
      sz += wprintln( sF(" <br>") + String(n) + F(" WIFI Networks Found") );
      sz += wprintln(  F(" <ul>") );
      sz += wprintln(  F("  <form method='POST' action='/connect'>") );
      for (int i = 0; i < n; ++i) {
          String TmpBuf = String(WiFi.SSID(i)) + F(" (") + String(WiFi.RSSI(i)) + F("dBm)");
          if (WiFi.encryptionType(i) == ENC_TYPE_NONE) TmpBuf += "*";
          yield();
          
          Serial.println ( "  " + TmpBuf);
          
          // Print SSID and RSSI for each Access Point found
          // Show Bar Graph
          sz += wprintln( sF("<br>") );
          sz += wprint  ( sF(" <meter value='") + String(WiFi.RSSI(i)) + F("'") );
          sz += wprintln( sF(" min='-100' low='-80' optimum='-60' high='-10' max='-10'>dBm</meter>") );
          // Show Text Data
          sz += wprintln( sF("   <input type='radio' name='nSsid' value='") + String(WiFi.SSID(i)) + F("'") );
          if ( String(WiFi.SSID(i)) == String(gSsid) ) {
            sz += wprint( F(" checked") );
          }
          sz += wprintln( F(">") );
          sz += wprint  ( TmpBuf ); // Append Connection-Record from above
          //sz += wprint ( sF(", - <a href='http://") + String(WiFi.SSID(i)) + F(".local/'>") + String(WiFi.SSID(i)) + F(".local</a>") );
          sz += wprintln( );
      }
      sz += wprintln(  F("") );
      sz += wprintln(  F("   <br><input type='hidden' name='ReStart' value='true' >") );
      sz += wprintln(  F("   <br><input type='text' name='nPasswd' value='' >") );
      sz += wprintln(  F("   <br>WAP PassPhrase (blank for open)*") );
      sz += wprintln(  F("   <br>Note: Connect Changes will Switch AP WiFi Channel") );
      sz += wprintln(  F("   <br><br><ul><input type='submit' value='Connect'></ul>") );
      sz += wprintln(  F("  </form>") );
      sz += wprintln(  F(" </ul>") );
    }
   
    PAGE_MONITOR_REPORT_END;
    
    return sz;
}

// ###########################################################    
// End
