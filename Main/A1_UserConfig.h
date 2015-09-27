// A1_UserConfig.h


#ifndef USERCONFIG_H
#define USERCONFIG_H


// Note: By my convention, All Global Variables and Constants start with a letter "g".

// Station Config
    String gSsid = "EBCON";        // Change to your SSID
    String gPasswd = "........";   // Change to your Passwd


// Default Unit ID "Node" Name, This will be changed in Setup
    char gDeviceName[16] = "Nod000000";


// Soft AP Config
    const String gSoftAPAddress = "192.168.4.1"; // Default Soft AP IP Address
    const int gDeviceApChannelDefault = 8; // Used when in SoftAP only Mode
    int gApChannel = gDeviceApChannelDefault;


// Initial Automatic Page Refresh Intervals
    int gAutoHomeRefresh = 180;
    int gAutoHelpRefresh = 180;
    int gAutoAdminRefresh = 180;


// Time ZONE
    const long gTimeZONE = (-7*HRs/SECs); // PDT


// Used to set LED state
    const boolean ON = 0;
    const boolean OFF = !ON;


// Pin numbers for LEDs
    const int gGrnLED = 0;
    const int gRedLED = 2;
    const int gBluLED = 4;
    boolean gRedLedState = OFF;

// My Known Esp Unit ID's
    const char* knowUnitID[] = {
      "1083.7620", "Esp13", // Nod129
      "1053.1281", "Esp11",
      "1669.8581", "Esp11",
      "1053.1277", "Esp11", // Nod168
    };
    
#endif // USERCONFIG_H

// ###########################################################
// End

