// AA_UtilsSend

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

/*
 * These Send Functions provide the WIFI Send Functions (APIs) for the rest of the Sketch.
 * 
 * Each API is written dependent on a Lower Level APIs, with only one API actually sending 
 *    data to WIFI.
 *    
 * The APIs are listed 'Lowest' to 'Highest" with their data flow dependence, they are:
 *
 *    _wSendCBuf();
 *         gServer.client().write();
 *
 *    _wprintstr();
 *        _wSendCBuf();   
 *
 *    wprint();
 *        _wSendCBuf();
 *
 *    wprintln();
 *        wprint();
 *
 *    wSendBuf_P();
 *        wprint();
 *        _wSendCBuf_();
 *
 *    wSendStr_P();
 *        wprint();
 * 
 * 
 *    
 */


// ###########################################################
//////////////////////////////////////////////////////////////
//
// This is a Private Send A Buffer Function, used to Transfer "Blocks" of Data to WIFI
//
long
//ICACHE_FLASH_ATTR
_wSendCBuf(const char* apBuf, long aLen, byte aFinish = SEND_WITH_BUFFERING)
{ 

    long sentSize = 0;
    long sentSizeTmp = 0;
    
    long p = 0;
    
    if (gMarkFlag == true) logFreeHeapMarkDn(); // Mark Start of Web Page
    
    int ErrorLoops = 0;
    while ( p < aLen ) {

      if (!gServer.client().remoteIP()) {
          Serial.println ( F(" Aborting Connection") ); // Abort for IPA: 0.0.0.0
          aFinish = SEND_FINISH;
          break;
      }

      long size2Send = aLen - p;
      if (size2Send > 1460) size2Send = 1460;
      sentSizeTmp = gServer.client().write(apBuf + p, size2Send);
      Serial.println ( sF(" Wifi, Sent Buf Size: ") + String(sentSizeTmp) );

      if (sentSizeTmp > 0) {
          ErrorLoops = 0;
          sentSize += sentSizeTmp;
          p += sentSizeTmp;
      } else {  // Try again.
          ErrorLoops++;
          if (ErrorLoops > 3) {
            aFinish = SEND_FINISH;
            Serial.println ( F(" Exiting Loop with: Error") ); 
            break;
          }
          for (int i = 5; i > 0; i--) { // While Trying again, delay for at least 500ms, with yield
            delay(100);
            yield();
          }
      }
    }

    if (aFinish == SEND_FINISH ) {
        gServer.client().flush();
        yield();
        gServer.client().stop();
        yield();
        
        Serial.println ( F(" Buf Connection Flushed and Closed") );
        
        if (gMarkFlag == false) logFreeHeapMarkUp(); // Mark End of Web Page
    }
     
    gSentSize += sentSize;
    
    return sentSize;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
String _WifiBuf = ""; // The Private WIFI Transfer Buffer
//
// This is a Private String-Buffer Function, used to build "Blocks" of Data
//
long
//ICACHE_FLASH_ATTR
_wprintstr(String aStr = "", byte aFinish = SEND_WITH_BUFFERING)
{ 
    long sz = 0;

    _WifiBuf += aStr;

    int size2Send = 0;
    if ( aFinish == SEND_WITH_BUFFERING) {
      size2Send = (_WifiBuf.length() / 1460) * 1460; // Buffer in Chunks of 1460 bytes
      if ( size2Send == 0 ) return 0;
    }
    else {
      size2Send = _WifiBuf.length();
    }
    sz += _wSendCBuf( _WifiBuf.c_str(), size2Send, aFinish );
    
    _WifiBuf.remove(0, sz);  // Remove the Head of the buffer that was sent to WIFI

    if ( aFinish != SEND_WITH_BUFFERING) _WifiBuf = "";
    
    return sz;
    
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// The rest of this APP uses this to send data to WIFI
//
long
//ICACHE_FLASH_ATTR
wprint(String aStr = "", byte aFinish = SEND_WITH_BUFFERING)
{
    return _wprintstr( aStr, aFinish );
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// The rest of this APP uses this to send data to WIFI
//
long
//ICACHE_FLASH_ATTR
wprintln(String aStr = "", byte aFinish = SEND_WITH_BUFFERING)
{   
    return wprint( aStr + F("\r\n"), aFinish );
}



// ###########################################################
//////////////////////////////////////////////////////////////
//
// This is a Buf_P Function, used to Transfer "Blocks" of Data to WIFI
//
long
//ICACHE_FLASH_ATTR
wSendBuf_P(PGM_P apBuf, long aLen, byte aFinish = SEND_WITH_BUFFERING)
{ 
  
      long sz = 0;
      
      sz += wprint( "", SEND_BUFFER_NOW ); // Send Anything already in String Buffer, First, NOW
      
      sz += _wSendCBuf( (char*) apBuf, aLen, aFinish );
      
      return sz;
}


// ###########################################################
//////////////////////////////////////////////////////////////
//
// This Function uses buffers to transfer "large blocks" of Text to WIFI
//
long
ICACHE_FLASH_ATTR
wSendStr_P( PGM_P apBuf, byte aFinish = SEND_WITH_BUFFERING )
{

      
      long sz = 0;
      long pBufLen = strlen_P(apBuf);
      char buf[1460/4 + 1];
      PGM_P pBuf = apBuf;

      while(pBuf < apBuf + pBufLen) {
        strncpy_P(buf, pBuf, sizeof(buf));
        buf[sizeof(buf)] = 0;
        pBuf += strlen(buf);
        sz += wprint( String (buf) );
      };
      
      sz += wprint( "", aFinish );
      
      return sz;
}

// ###########################################################
// End


