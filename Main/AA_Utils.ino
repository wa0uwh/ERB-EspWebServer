// AA_SupportFunc

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
String _WifiBuf = ""; // The Private WIFI Transfer Buffer
//
// This is a Private String-Buffer Function, used to Transfer "Blocks" of Data to WIFI
//
long
//ICACHE_FLASH_ATTR
_wprintstr(String aStr = "", boolean aFinish = false)
{ 
    long sentSize = 0;
    // static String _WifiBuf = ""; // This Static will not compile ??
    
    if (gMarkFlag == true) logFreeHeapMarkDn(); // Mark Start of Web Page
    
    _WifiBuf += aStr;
    yield();
         
    int ErrorLoops = 0;
    while (_WifiBuf.length() >= 1460 || aFinish) {
        logFreeHeap();
        
        int sentSizeTmp = gServer.client().print(_WifiBuf.substring(0, 1460));
        yield();
        
        gSentSize += sentSizeTmp;
        sentSize  += sentSizeTmp;
        yield();
        
        _WifiBuf.remove(0,sentSizeTmp);  // Remove the Head of the buffer that was sent to WIFI
        Serial.println ( sF(" Wifi, Sent Size: ") + String(sentSizeTmp) );
        
        if ( !_WifiBuf.length() ) break;
   
        if (sentSizeTmp > 0) {
           ErrorLoops = 0;
        } else {  // Try again.
            ErrorLoops++;
            if (ErrorLoops > 3) {
              aFinish = true;
              Serial.println ( F(" Exiting Loop with: Error") ); 
              break;
            }
            for (int i = 5; i > 0; i--) { // While Trying again, delay for at least 500ms, with yield
              delay(100);
              yield();
            }
        }
        yield();
    }
    
    if (aFinish == true) {
      
        gServer.client().flush();
        yield();
        gServer.client().stop();
        yield();
        _WifiBuf = "";
        
        Serial.println ( F(" Connection Flushed and Closed") ); 
    
        if (gMarkFlag == false) logFreeHeapMarkUp(); // Mark End of Web Page
    }
        
    return sentSize;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// The rest of this APP uses this to send data to WIFI
//
long
//ICACHE_FLASH_ATTR
wprint(String aStr = "", boolean aFinish = false)
{ 
    long sz = 0;
    
    sz += _wprintstr( aStr, aFinish);
    
    return sz;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// The rest of this APP uses this to send data to WIFI
//
long
//ICACHE_FLASH_ATTR
wprintln(String aStr = "", boolean aFinish = false)
{   
    long sz = 0;
    
    aStr += F("\r\n");
    sz += wprint( aStr, aFinish);
    
    return sz;
}
  
// ###########################################################
//////////////////////////////////////////////////////////////
//
// Html Page Header: Common Header Text for all Web Pages
//
long
ICACHE_FLASH_ATTR
htmlPageHeader( String aTitle, int aAutoRefresh = 0, String aURL = "/" )
{
    long sz = 0;
    
    gPageStartTime = millis();
    
    // Html Page Header: Common Header for all Web Pages
    sz += wprintln( F("<!DOCTYPE html>") );
    sz += wprintln( F("<html>") );
    sz += wprintln( F("  <head>") );
    sz += wprintln( sF("    <title>") + aTitle + F("</title>") );
    if (aAutoRefresh > 0 ) sz += wprintln( sF("    <meta http-equiv='refresh' content='") + String(aAutoRefresh) + F(";URL=") + aURL + F("'/>") );
    sz += wprintln( F("    <meta name='viewport' content='width=device-width, initial-scale=1'/>") );
    sz += wprintln( F("    <meta http-equiv='Pragma' content='no-cache'>") );
    sz += wprintln( F("    <link rel='shortcut icon' href='http://espressif.com/favicon.ico'>") ); // Redirect requests for "/favicon.ico" 
    sz += wprintln( F("    <style>") );
    sz += wprintln( F("       body {background-color: #ddddee;}") );
    sz += wprintln( F("    </style> ") );
    sz += wprintln( F("  </head>") );
    sz += wprintln( );
    sz += wprintln( F(" <!-- Body -->") );
    sz += wprintln( F(" <body>") );
    
    return sz;
}
  
// ###########################################################
//////////////////////////////////////////////////////////////
//
// Html Page Title: Common Title Text for all Web Pages
//
long
ICACHE_FLASH_ATTR
htmlPageTitle( String aNodeName, String aTitle )
{
    long sz = 0;
    sz += wprintln( F("<center><h1>") );
    sz += wprintln( String(aNodeName) );
    sz += wprintln( F(" - ") );
    sz += wprintln( String(aTitle) );
    sz += wprintln( F("<br><font size='2'>ERB's Esp8266 Experimental Web Server</font>") );
    sz += wprintln( F("</h1></center>") );
    return sz;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Html Page Footer: Common Footer and Trailer-Text for all Web Pages
//
long
ICACHE_FLASH_ATTR
htmlPageFooter()
{
    long sz = 0;
    
    sz += wprintln( );
    sz += wprintln( F("<!-- Footer -->") );
    sz += wprintln( F("<center>") );
    sz += wprintln( F("<hr width='90%'> ") );
    
    sz += wprintln( F("Created at: <b>") );
    sz += wprint  ( String ( (gSentSize / ((millis() - gPageStartTime)/1000.0)/1000.0), 1 ) );
    sz += wprint  ( F("</b> MB/Sec") );
    sz += wprint  ( F(" - <b>") );
    sz += wprint  ( String ( (millis() - gPageStartTime)/1000.0, 3 ) );
    sz += wprint  ( F("</b> CPU Secs - <b>") );
    sz += wprint  ( gLoadAvgS );  // Pseudo Load Average
    sz += wprintln( F("</b> LoadAvg (pseudo)") );
    sz += wprintln( F("<br>") );
    sz += wprintln( F("<br>") );
    
    sz += wprintln( F("Powered by: <a href='http://espressif.com/'>Esp8266</a>") );
    sz += wprintln( F("<br>") );

    sz += wprintln( F("Contact Mgr at: <a href='tel://202-555-1212'>202-555-1212</a>") );
    sz += wprintln( F("<br>") );
    
    sz += wprintln( F("<a href='/info'>Copyright 2015</a>") );    
    sz += wprintln( F("<br>") );
    
    sz += wprintln( F("<br>-") );
    sz += wprintln( F("</center>") );
    
    sz += wprintln( );
    sz += wprintln( F(" <!-- Trailer -->") );
    sz += wprintln( F(" </body>") );
    sz += wprintln( F("</html>") );
    
    return sz;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// A Previous Page Director
//
void
ICACHE_FLASH_ATTR
pageDirector(int page = gCurrentPage )
{
    switch ( page ) {
      case ADMINPAGE: handleAdminPage(); break;
      case HELPPAGE:  handleHelpPage();  break ;
      case INFOPAGE:  handleInfoPage();  break ;
      case HOMEPAGE:  // Fall thru to Default
      default:        handleHomePage();
    }
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Toggles Page Verbose Mode
//
void
ICACHE_FLASH_ATTR
toggleVerboseMode()
{
    gVerbose = !gVerbose;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Sets Page Verbose Mode
//
void
ICACHE_FLASH_ATTR
verboseMode(boolean aMode = true)
{
    gVerbose = aMode;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Provides UpTime in Seconds
//
unsigned long
upTime()
{
  return millis() / 1000;
}
 

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Converts Uptime to a String Value, Days, Hours:Minutes:Seconds
//
String
upTimeStr()
{
//    char buf[PBUFSIZE];
    
    int uptimesec = upTime();
    int uptimemin = uptimesec / 60;
    int uptimehr  = uptimemin / 60;
    int uptimeday = uptimehr  / 24;
    yield();

    uptimesec %= 60;
    uptimemin %= 60;
    uptimehr  %= 24;

    snprintf( gTmpBuf, sizeof(gTmpBuf),
        FMT("%d Days, %02d:%02d:%02d"), uptimeday, uptimehr, uptimemin, uptimesec );
    yield();
    
    return gTmpBuf;
} 

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Converts MAC Address to String
//
String
ICACHE_FLASH_ATTR
mac2str( byte mac[6] )
{
    
    snprintf(gTmpBuf, sizeof(gTmpBuf), 
        FMT("%X:%X:%X:%X:%X:%X"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    yield();
    
    return gTmpBuf;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Converts ID to HEX String
//
String
ICACHE_FLASH_ATTR
id2hex( int id )
{
    
    snprintf(gTmpBuf, sizeof(gTmpBuf), 
        FMT("%X"), id);
    yield();
    
    return gTmpBuf;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Converts numeric IPA to String
//
String
ICACHE_FLASH_ATTR
ipa2str( IPAddress ipa )
{
    
    snprintf(gTmpBuf, sizeof(gTmpBuf), 
        FMT("%d.%d.%d.%d"), ipa[0], ipa[1], ipa[2], ipa[3]);
    yield();
    
    return gTmpBuf;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Logs the FreeHeap Data, this is called periodically
//
int
logFreeHeap()
{
  gFreeHeapLog[gFreeHeapLogIndex++ % sizeof(gFreeHeapLog)] = (byte) constrain(ESP.getFreeHeap()/100, 0, 255);
  // Serial.println ( sF(" FreeHeap: ") + String( ESP.getFreeHeap()/1000.0, 1 ) ); // Debug
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Creates/Inserts a Tick Mark in the FreeHeap Data Record
//
int
_logFreeHeapMark(int aDir)
{
    // Insert a visible Mark in the Free Heap Log
    const int MarkAmp = 16; // Mark Amplitude
    aDir = constrain(aDir, -1, +1);
    byte Save = gFreeHeapLog[(gFreeHeapLogIndex + sizeof(gFreeHeapLog) -1) % sizeof(gFreeHeapLog)];
    gFreeHeapLog[(gFreeHeapLogIndex++) % sizeof(gFreeHeapLog)] = (byte) constrain(Save + MarkAmp*aDir, 0, 255); // The Mark
//  gFreeHeapLog[(gFreeHeapLogIndex++) % sizeof(gFreeHeapLog)] = (byte) constrain(Save + MarkAmp*aDir, 0, 255); // Make it Wider
    gFreeHeapLog[(gFreeHeapLogIndex++) % sizeof(gFreeHeapLog)] = Save;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Marks the End of a Web Page in Free Heap Log
//
int
logFreeHeapMarkUp()
{
    // Put a visible Mark "up" in the Free Heap Log
    _logFreeHeapMark(+1);
    gMarkFlag = true;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Marks the Beginning of a Web Page in Free Heap Log
//
int
logFreeHeapMarkDn() 
{
    // Put a visible Mark "dn" in the Free Heap Log
    _logFreeHeapMark(-1);
    gMarkFlag = false;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// Provides easy access to Global Load Avg Value
//
String
//ICACHE_FLASH_ATTR
pseudoLoadAvgStr() {
      return gLoadAvgS;
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// Logs a Pseudo Load Average, this is called periodically
// This is NOT real, it is based on just some timing numbers
// A better method of computing Load Average should be found
//
void
//ICACHE_FLASH_ATTR
logPseudoLoadAvg() {
  
   // Computed as: Load Avg = (ExpectedCount / gLoadAvgCount) - 1
   
      #define ExpectedCount (92000)
        
      if (gLoadAvgL <= 0) gLoadAvgL = ExpectedCount; // Initialization
      
      
      long curDeltaL = gIdleL - gLoadAvgCountL;
      
      gLoadAvgL = ( (gLoadAvgL * 7) + (curDeltaL * 3) ) / 10; // Weigthed 7:3 Running Average
      
      gLoadAvgS = String ( ((1.0 * ExpectedCount / gLoadAvgL) - 1 ), 2 );
      
//      // For Debug
//      Serial.println ( \
//           sF("curDeltaL: ") + curDeltaL \
//          + F(", gLoadAvgL: ") + gLoadAvgL \
//          + F(", gLoadAvgS: ") + gLoadAvgS );
      
      gLoadAvgCountL = gIdleL;
      
      return;
}

// ###########################################################
// End
