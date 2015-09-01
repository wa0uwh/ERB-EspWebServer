// RefreshButton

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
