// FarmImage

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

#include "Farm01.h"
 

// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function provides the Content for the URL Requsest from WIFI
//
long
ICACHE_FLASH_ATTR
farmImage()
{
    long sz = 0;
    
    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();

    sz += wSendBuf_P(gFarm01, sizeof(gFarm01));
    
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
handleFarmImage()
{
    long sz = 0;
    gSentSize = 0;
    
    digitalWrite ( gBluLED, ON );
      gHits++;
      
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: image/jpeg") );
      sz += wprintln( ); // A Blank Line
 
      sz += farmImage();
           
      sz += wprint( "", SEND_FINISH ); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();
      
    digitalWrite ( gBluLED, OFF );
    yield();
}

// ###########################################################
// End
