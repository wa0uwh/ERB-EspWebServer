// InfoPage

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
// This Function uses "small" buffers to transfer "large blocks" of Text to WIFI
//
long
ICACHE_FLASH_ATTR
sendAsIs_P( PGM_P aPCom )
{
      long sz = 0;
      PGM_P pC = aPCom;
      long pLen = strlen_P(aPCom);
      char buf[64];
      
      while(pC < aPCom + pLen) {
        strncpy_P(buf, pC, sizeof(buf));
        buf[sizeof(buf)] = 0;
        pC += strlen(buf);
        sz += wprint( String (buf) );
      };
      
      return sz;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function Formats the Copyright Comment Text for transfer to WIFI
//
long
ICACHE_FLASH_ATTR
infoPage()
{
    long sz = 0;
    
    DEBUG_MONITOR_REPORT_START();
    
    DEBUG_MONITOR_REPORT_ARGS();

    // Generate Html Header
    sz += htmlPageHeader( gDeviceName, -1 );
    
    // Page Content Starts here
    
    // Page Title Line
    sz += htmlPageTitle( gDeviceName, F("Info") );
    
    // Navigator
    sz += navigator();
   
    sz += wprintln( );
    sz += wprintln( F("<!-- Page Content -->") );
    
    sz += wprintln(  F("<blockquote>") );
    {
      sz += wprintln( );
      sz += wprintln( F("<!-- Introduction -->") );
      sz += wprintln( F("<b>Introduction:</b>") );
      sz += wprintln( F("<blockquote><pre>") );
        sz += sendAsIs_P( INTRODUCTION );
        sz += wprintln( );
      sz += wprintln( F("</pre></blockquote>") );
      
      
      sz += wprintln( );
      sz += wprintln( F("<!-- Esp8266 Server Photo -->") );
      sz += wprintln( F("<center>") );
      sz += wprint  ( F("<a href='http://goo.gl/6iaPMf'>") );
      sz += wprint  ( F("<img") );
      sz += wprint  ( F(" src='http://goo.gl/6iaPMf'") ); // Photo Src
      sz += wprint  ( F(" alt='Server Photo'") );
      sz += wprint  ( F(" width='400'>") );
      sz += wprintln( F("</a>") );
      sz += wprintln( F("<br><b>Esp8266 WEB Server Farm</b>") );
      sz += wprintln( F("</center>") );
      
      sz += wprintln( );  
      sz += wprintln( F("<!-- Copyright -->") );
      sz += wprintln( F("<b>Copyright:</b>") );
      sz += wprintln( F("<blockquote><pre>") );
        sz += sendAsIs_P( COPYRIGHT1 );
        sz += sendAsIs_P( COPYRIGHT2 );
        sz += sendAsIs_P( COPYRIGHT3 );
        sz += wprintln( );  
      sz += wprintln( F("</pre></blockquote>") );

//      sz += wprintln( );  
//      sz += wprintln( F("<!-- Test Pattern Transfer -->") );
//      sz += wprintln( F("<b>Test Transfer:</b>") );
//      sz += wprintln( F("<blockquote><pre>") );
//        // 5 Groups of 10Kb = 50Kb 
//        sz += sendAsIs_P( TEST_10Kb );   // A 10Kb Test
//        sz += sendAsIs_P( TEST_10Kb );   // A 10Kb Test
//        sz += sendAsIs_P( TEST_10Kb );   // A 10Kb Test
//        sz += sendAsIs_P( TEST_10Kb );   // A 10Kb Test
//        sz += sendAsIs_P( TEST_10Kb );   // A 10Kb Test\
//        sz += wprintln( );  
//      sz += wprintln( F("</pre></blockquote>") );

    }
    sz += wprintln( F("</blockquote>") );


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
handleInfoPage()
{
    long sz = 0;
    gSentSize = 0;
  
    gCurrentPage = INFOPAGE;
    
    digitalWrite ( gGrnLED, ON );
      gHits++;
    
      // HTTP Header
      sz += wprintln( F("HTTP/1.1 200 OK") );
      sz += wprintln( F("Content-Type: text/html") );
      sz += wprintln( );
      
      sz = infoPage();
   
      sz += wprint( "", true); // Final Packet

      DEBUG_MONITOR_REPORT_TOTAL();
    
    digitalWrite ( gGrnLED, OFF );
    yield();
}

// ###########################################################
// End
