// A_Handlers

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
// This Function provides links to Handlers for the URL Requsest from WIFI
//
void
ICACHE_FLASH_ATTR
initHandlers()
{
  
    // Page Handlers
    gServer.on ( "/",              handleHomePage );
    gServer.on ( "/home",          handleHomePage );
    gServer.on ( "/help",          handleHelpPage );
    gServer.on ( "/admin",         handleAdminPage );
    gServer.on ( "/info",          handleInfoPage );
    gServer.on ( "/test.svg",      handleLineGraph );
    gServer.on ( "/freeheap.svg",  handleLineGraphFH );
    
    gServer.on ( "/clock.svg",     handleClock );
    gServer.on ( "/clock2.svg",    handleClock );
    gServer.on ( "/clock3.svg",    handleClock );
    gServer.on ( "/inst.svg",      handleInst );
    gServer.on ( "/inst2.svg",     handleInst );
    gServer.on ( "/inst3.svg",     handleInst );
    
    gServer.on ( "/bar.svg",       handleBar );
    gServer.on ( "/sliderbar",     handleSliderBar );
    gServer.on ( "/visitormap",    handleVisitorMap );
    
    // Verbose Option, Hidden from HELP
    gServer.on ( "/on/vismap",      []() { gHits++; gVisitorMapState = true;  pageDirector();} );
    gServer.on ( "/off/vismap",     []() { gHits++; gVisitorMapState = false; pageDirector();} );
    gServer.on ( "/a/vismap",       []() { gHits++; gVisitorMapStyle = 1; pageDirector();} );
    gServer.on ( "/b/vismap",       []() { gHits++; gVisitorMapStyle = 2; pageDirector();} );
    
    // Verbose Option
    gServer.on ( "/t/v",           []() { gHits++; toggleVerboseMode(); pageDirector();} );
    gServer.on ( "/s/v",           []() { gHits++; verboseMode(true);   pageDirector();} );
    gServer.on ( "/c/v",           []() { gHits++; verboseMode(false);  pageDirector();} );
    
    // LED Control
    gServer.on ( "/t/redled",      []() { gHits++; redLed_toggle(); pageDirector();} );
    gServer.on ( "/on/redled",     []() { gHits++; redLed(ON);      pageDirector();} );
    gServer.on ( "/off/redled",    []() { gHits++; redLed(OFF);     pageDirector();} );
    
    // Auto Page Update Options
    gServer.on ( "/home/auto_on",  []() { gHits++; gAutoHome =   true; pageDirector();} ); // Hidden from Help
    gServer.on ( "/home/auto_off", []() { gHits++; gAutoHome =  false; pageDirector();} ); // Hidden from Help
    gServer.on ( "/help/auto_on",  []() { gHits++; gAutoHelp =   true; pageDirector();} ); // Hidden from Help
    gServer.on ( "/help/auto_off", []() { gHits++; gAutoHelp =  false; pageDirector();} ); // Hidden from Help
    gServer.on ( "/admin/auto_on", []() { gHits++; gAutoAdmin =  true; pageDirector();} ); // Hidden from Help
    gServer.on ( "/admin/auto_off",[]() { gHits++; gAutoAdmin = false; pageDirector();} ); // Hidden from Help
    
    // Querys
    gServer.on ( "/q/mdns",        []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gDeviceName) + F(".local"));} );
    gServer.on ( "/q/ssid",        []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(WiFi.SSID()));} );
    gServer.on ( "/q/rssi",        []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(WiFi.RSSI()) + F("dBm"));} );
    gServer.on ( "/q/stnipa",      []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + ipa2str(WiFi.localIP()));} );
    gServer.on ( "/q/stnmac",      []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + mac2str(WiFi.macAddress(gMacBuf)));} );
    gServer.on ( "/q/gateway",     []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + ipa2str(WiFi.gatewayIP()));} );
    gServer.on ( "/q/netmask",     []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + ipa2str(WiFi.subnetMask()));} );
    gServer.on ( "/q/myipa",       []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + ipa2str(gServer.client().remoteIP()));} );
    
    gServer.on ( "/q/restarts",    []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gWifiReStarts));} );
    gServer.on ( "/q/redled",      []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gRedLedState == ON ? "ON" : "OFF"));} );
    gServer.on ( "/q/uptime",      []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(upTime()));} );
//  gServer.on ( "/q/utctime",     []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(utcTime()));} );
    gServer.on ( "/q/epoch",       []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(epoch()));} );
    gServer.on ( "/q/loadavg",     []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gLoadAvgS));} );
    gServer.on ( "/q/battvolt",    []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(readvdd33()/1000.0, 3));} );
    gServer.on ( "/q/freeheap",    []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + ESP.getFreeHeap());} );
    gServer.on ( "/q/hits",        []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gHits));} );
    
    gServer.on ( "/q/id",          []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(ESP.getChipId() / 10000.0, 4));} );
    gServer.on ( "/q/idx",         []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(id2hex(ESP.getChipId())));} );
    gServer.on ( "/q/flashid",     []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(ESP.getFlashChipId() / 10000.0, 4));} );
    gServer.on ( "/q/flashsize",   []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(ESP.getFlashChipSize() / 1000.0, 1));} );
    gServer.on ( "/q/flashspd",    []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(ESP.getFlashChipSpeed() / 1000000.0, 1));} );
    
    gServer.on ( "/q/rev",         []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gRev));} );
    
    gServer.on ( "/q/apssid",      []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gDeviceName));} );
    gServer.on ( "/q/apipa",       []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(ipa2str(WiFi.softAPIP())));} );
    gServer.on ( "/q/apmac",       []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + mac2str(WiFi.softAPmacAddress(gMacBuf)));} );
    gServer.on ( "/q/apchannel",   []() { gHits++; gServer.send ( 200, gTextPlain, String(gHits) + ": " + String(gApChannel));} );
    
    
    // Admin functions
    gServer.on ( "/scanwifi", handleScan );
    gServer.on ( "/connect",  handleConnect ); // Hidden from HELP
    
    gServer.onNotFound ( handleNotFound );
}


// ###########################################################
// End
