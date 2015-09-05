// mDNS

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
// mDNS Resonder
//
void
ICACHE_FLASH_ATTR
initMDNS()
{
    
    digitalWrite ( gBluLED, ON );
      if ( gMDns.begin ( gDeviceName, WiFi.localIP() ) ) {
         Serial.println ( sF("MDNS Responder Started for: ") + String( gDeviceName ) );
         yield();
         gMDnsResponderReady = true;
      }  
    digitalWrite ( gBluLED, OFF );
    yield();
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Updates mDNS
//
void
ICACHE_FLASH_ATTR
updateMDNS()
{
    
    const unsigned long schdInc = 5 * MINs;
    
    if (!gMDnsResponderReady) { initMDNS(); return; }
    
    if (gNextMDnsSchd > millis()) return; // Abort, wait for next Run
     
    gNextMDnsSchd = millis() + schdInc;  // Schedule Next Run
     
    digitalWrite ( gBluLED, ON );
       gMDns.update();
       yield();
       Serial.println ( F("\nMDMS Update Sent ...") ); 
    digitalWrite ( gBluLED, OFF );
    yield();
     
}


// ###########################################################
// End
