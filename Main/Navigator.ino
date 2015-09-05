// Navigator

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
navigator( )
{
    long sz = 0;
    
    String timeUTCBuf = "";
    
    sz += wprintln( );
    sz += wprintln( F("<!-- Navigator -->") );
    sz += wprintln( F("<table border='0' width='100%'>") );
    sz += wprintln( F(" <tr><td align='left' width='40%'>") );
    
    sz += wprint  ( F(" | <a href='/'>Home</a>") );
    sz += wprint  ( F(" | <a href='/help'>Help</a>") );
    sz += wprint  ( F(" | <a href='/admin'>Admin</a>") );
    sz += wprint  ( F(" | <a href='/info'>Info</a>") );
    sz += wprintln( F(" |") );
    
    sz += wprintln( F(" </td><td align='center'>") );
    sz += wprintln( timeUTC(timeUTCBuf, gTimeZONE) ); 
    sz += wprintln( F(" PDT</td><td align='right' width='40%'>") );
    
    sz += wprint  ( F(" | <a href='http://espressif.com/'>MFR</a>") );
    sz += wprint  ( F(" | <a href='http://www.esp8266.com/'>Forum</a>") );
    sz += wprint  ( F(" | <a href='http://bbs.espressif.com/'>BBS</a>") );
    sz += wprint  ( F(" | <a href='http://neilkolban.com/tech/esp8266/'>TheBOOK</a>") );
    sz += wprint  ( F(" | <a href='https://github.com/esp8266/Arduino'>GitHub</a>") );
    sz += wprintln( F(" |") );
    
    sz += wprintln( F(" </td></tr>") );
    sz += wprintln( F("</table>") );
    
    sz += wprintln(sF("<center>") + String(ESP.getFreeHeap() / 1000.0, 3) + " - " + String(gHits) + " - " + String(gRev) + F("</center>") );
    
    return sz;
}

// ###########################################################    
// End
