// NTP_Client

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
 

unsigned int localPort = 2390;        // local port to listen for UDP packets

//IPAddress gTimeServer(129, 6, 15, 28);    // time.nist.gov NTP server
IPAddress gTimeServer(70, 35, 113, 43);   // time.nist.gov NTP server
//IPAddress gTimeServer(192, 168, 43, 195);   // time.nist.gov NTP server
//IPAddress gTimeServer(192, 168, 2, 15);     // local NTP server

const int NTP_PACKET_SIZE = 48;       // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE];  // buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;


// ###########################################################
//////////////////////////////////////////////////////////////
//
unsigned long
ICACHE_FLASH_ATTR
getEpochDelta()
{
  
  Serial.println ( F("\nStarting Connection to NTP Server ...") );
  udp.begin(localPort);
  yield();
  
  gEpochAvailable = false;

  int packetLength = udp.parsePacket();
  int loops = 0;
  while (!packetLength) {
    if (!(loops % 15)) {
      Serial.println ( " " );
      sendNTPpacket();
      Serial.print ( F(" No NTP Packets Received Yet "));
    }
    loops = ++loops % 30;
    if (!loops) { Serial.println ( "." ); Serial.print ( F("Re-Scheduling NTP Request\n")); return 0;}
    Serial.print ( "." );
    gServer.handleClient();
    delay(100);
    gServer.handleClient();
    yield();
    packetLength = udp.parsePacket();
    gServer.handleClient();
    delay(250);
    yield();
  }

    unsigned long upTimeStamp = upTime();
    
    Serial.print( F("\n NTP Packet Received, Length: ") );
    Serial.println ( packetLength );
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    yield();

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print( F(" Seconds since Jan 1 1900 = " ) );
    Serial.println ( secsSince1900 );
    yield();
    
    if (! secsSince1900 ) return 0;

    // now convert NTP time into everyday time:
    Serial.print( F(" Unix time = ") );
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    gEpochDelta = secsSince1900 - seventyYears - upTimeStamp;
    // print Unix time:
    Serial.println ( gEpochDelta );
    
    gEpochAvailable = true;
    yield();
    
    return gEpochDelta;
  }
 
// ###########################################################
//////////////////////////////////////////////////////////////
//
unsigned long
epoch()
{
  if (gEpochDelta) return gEpochDelta + upTime();
  return 0;
}

 
// ###########################################################
//////////////////////////////////////////////////////////////
// 
void
ICACHE_FLASH_ATTR
updateNTP()
{
    unsigned long schdInc = 30 * MINs;
        
    if (gEpochAvailable == false) schdInc = 1 * MINs;
    
    if (gNextNTPSchd > millis()) return; // Abort, wait for next Run
     
    gNextNTPSchd = millis()+ schdInc;  // Schedule Next Run
    
    if (gEpochAvailable == false) getEpochDelta();
}

// ###########################################################
//////////////////////////////////////////////////////////////
//   
String &
timeUTC(String & timeBuf, unsigned long timezone)
{
    
    if (!epoch()) return timeBuf;
    
    unsigned long timesec  = epoch() + timezone;
    unsigned long timemin  = timesec / 60;
    unsigned long timehr   = timemin / 60;
    unsigned long timeday  = timehr  / 24;
    unsigned long timewday = timeday / 24;
    yield();

    timesec  %= 60;
    timemin  %= 60;
    timehr   %= 24;
    timewday %=  7; timewday += 3;

    snprintf( gTmpBuf, sizeof(gTmpBuf),
        FMT("%02d:%02d:%02d"), (int) timehr, (int) timemin, (int) timesec );
    timeBuf = gTmpBuf;
    return timeBuf;
  
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
// send an NTP request to the time server at the given address
//
unsigned long
ICACHE_FLASH_ATTR
sendNTPpacket()
{
    
  Serial.print( F(" Sending NTP Request Packet, to: ") );
  Serial.println ( gTimeServer );
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;   // 1
  packetBuffer[13]  = 0x4E; // N, 78
  packetBuffer[14]  = 49;   // 1
  packetBuffer[15]  = 52;   // 4

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(gTimeServer, 123); //NTP requests are to port 123
  Serial.println ( F(" Connecting to NTP Server Port") );
  yield();
  
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  Serial.println ( F(" NTP UDP Packet Sent") );
  yield();
  
  udp.endPacket();
  Serial.println ( F(" NTP UDP Packet End") );
  yield();
}

// ###########################################################
// End
