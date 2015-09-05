// RefreshButton

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
long
ICACHE_FLASH_ATTR
reFreshButton(String link)
{
    long sz = 0;

    sz += wprintln( sF(" <form method='GET' action='") + String(link) + F("'>") );
    sz += wprintln( F("  <input type='submit' name='' value='Refresh'/>") );
    sz += wprintln( F(" </form>") );
    
    return sz;
}

// ###########################################################
// End
