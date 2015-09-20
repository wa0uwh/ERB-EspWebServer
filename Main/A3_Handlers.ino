// A3_Handlers

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
    gServer.on ( "/farm01.jpg",    handleFarmImage );
    
    // Verbose Option, Hidden from HELP
    gServer.on ( "/on/vismap",      []() { gHits++; DMRAF(); gVisitorMapState = true;  pageDirector();} );
    gServer.on ( "/off/vismap",     []() { gHits++; DMRAF(); gVisitorMapState = false; pageDirector();} );
    gServer.on ( "/a/vismap",       []() { gHits++; DMRAF(); gVisitorMapStyle = 1; pageDirector();} );
    gServer.on ( "/b/vismap",       []() { gHits++; DMRAF(); gVisitorMapStyle = 2; pageDirector();} );
    
    // Verbose Option
    gServer.on ( "/t/v",           []() { gHits++; DMRAF(); toggleVerboseMode(); pageDirector();} );
    gServer.on ( "/s/v",           []() { gHits++; DMRAF(); verboseMode(true);   pageDirector();} );
    gServer.on ( "/c/v",           []() { gHits++; DMRAF(); verboseMode(false);  pageDirector();} );
    
    // LED Control
    gServer.on ( "/t/redled",      []() { gHits++; DMRAF(); redLed_toggle(); pageDirector();} );
    gServer.on ( "/on/redled",     []() { gHits++; DMRAF(); redLed(ON);      pageDirector();} );
    gServer.on ( "/off/redled",    []() { gHits++; DMRAF(); redLed(OFF);     pageDirector();} );
    
    // Auto Page Update Options
    gServer.on ( "/home/auto_on",  []() { gHits++; DMRAF(); gAutoHomeRefresh  = 60; pageDirector();} ); // Hidden from Help
    gServer.on ( "/home/auto_off", []() { gHits++; DMRAF(); gAutoHomeRefresh  =  0; pageDirector();} ); // Hidden from Help
    gServer.on ( "/help/auto_on",  []() { gHits++; DMRAF(); gAutoHelpRefresh  = 60; pageDirector();} ); // Hidden from Help
    gServer.on ( "/help/auto_off", []() { gHits++; DMRAF(); gAutoHelpRefresh  =  0; pageDirector();} ); // Hidden from Help
    gServer.on ( "/admin/auto_on", []() { gHits++; DMRAF(); gAutoAdminRefresh = 60; pageDirector();} ); // Hidden from Help
    gServer.on ( "/admin/auto_off",[]() { gHits++; DMRAF(); gAutoAdminRefresh =  0; pageDirector();} ); // Hidden from Help
    
    // Querys
    gServer.on ( "/q/mdns",        []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gDeviceName) + F(".local"));} );
    gServer.on ( "/q/ssid",        []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(WiFi.SSID()));} );
    gServer.on ( "/q/rssi",        []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(WiFi.RSSI()) + F("dBm"));} );
    gServer.on ( "/q/stnipa",      []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + ipa2str(WiFi.localIP()));} );
    gServer.on ( "/q/stnmac",      []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + mac2str(WiFi.macAddress(gMacBuf)));} );
    gServer.on ( "/q/gateway",     []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + ipa2str(WiFi.gatewayIP()));} );
    gServer.on ( "/q/netmask",     []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + ipa2str(WiFi.subnetMask()));} );
    gServer.on ( "/q/myipa",       []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + ipa2str(gServer.client().remoteIP()));} );
    
    gServer.on ( "/q/restarts",    []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gWifiReStarts));} );
    gServer.on ( "/q/redled",      []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gRedLedState == ON ? "ON" : "OFF"));} );
    gServer.on ( "/q/uptime",      []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(upTime()));} );
//  gServer.on ( "/q/utctime",     []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(utcTime()));} );
    gServer.on ( "/q/epoch",       []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(epoch()));} );
    gServer.on ( "/q/loadavg",     []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gLoadAvgS));} );
    gServer.on ( "/q/battvolt",    []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(readvdd33()/1000.0, 3));} );
    gServer.on ( "/q/freeheap",    []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + ESP.getFreeHeap());} );
    gServer.on ( "/q/hits",        []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gHits));} );
    
    gServer.on ( "/q/id",          []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(ESP.getChipId() / 10000.0, 4));} );
    gServer.on ( "/q/idx",         []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(id2hex(ESP.getChipId())));} );
    gServer.on ( "/q/flashid",     []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(ESP.getFlashChipId() / 10000.0, 4));} );
    gServer.on ( "/q/flashsize",   []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(ESP.getFlashChipSize() / 1000.0, 1));} );
    gServer.on ( "/q/flashspd",    []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(ESP.getFlashChipSpeed() / 1000000.0, 1));} );
    
    gServer.on ( "/q/rev",         []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gRev));} );
    
    gServer.on ( "/q/apssid",      []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gDeviceName));} );
    gServer.on ( "/q/apipa",       []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(ipa2str(WiFi.softAPIP())));} );
    gServer.on ( "/q/apmac",       []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + mac2str(WiFi.softAPmacAddress(gMacBuf)));} );
    gServer.on ( "/q/apchannel",   []() { gHits++; DMRAF(); gServer.send ( 200, TEXT_PLAIN, String(gHits) + ": " + String(gApChannel));} );
    
    
    // Admin functions
    gServer.on ( "/scanwifi", handleScan );
    gServer.on ( "/connect",  handleConnect ); // Hidden from HELP
    
    gServer.onNotFound ( handleNotFound );
}


// ###########################################################
// End
