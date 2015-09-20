// A2_UtilsSend.h

#ifndef UTILSSEND_H
#define UTILSSEND_H


// ###########################################################
//////////////////////////////////////////////////////////////
const char THIS_APP_WIFI_SEND_API[] PROGMEM = R"(
    /*
     * These Send Functions provide this App with WIFI Send Functions (APIs),
     *    only the four High Level functions should be used, and only these.
     *
     * Each API is written dependent on a Lower Level APIs, with only one API actually
     *    sending data to WIFI.
     *
     * The APIs are listed 'Highest' to 'Lowest" with their data flow dependence:
     *
     * High Level:
     *
     *    wprintln();                      // High Level, App API to Print a String with NewLine
     *        wprint();                    // Call App API Print a String
     *
     *    wprint();                        // High Level, App API to Print a String
     *        _wBufStr();                  // Call Low Level Private C Buffer
     *
     *    wSendCBuf_P();                   // High Level, App API to Send a P C buffer
     *        wprint();                    // Call App API to Print Current Content of String Buffer
     *        _wSendCBuf();                // Call Low Level Private C Buffer
     *
     *    wSendStr_P();                    // High Level, App API to Send a P Strings
     *        wprint();                    // Call App API Print a String
     *
     * Low Level:
     *
     *    _wBufStr();                      // Low Level, Private Send Str Buffer
     *        _wSendCBuf();                // Call Low Level Private C Buffer
     *
     *    _wSendCBuf();                    // Low Level, Private Send C Buffer
     *         gServer.client().write();   // This is the single API to WIFI Transmit Function !!!!
     *
     *
     */
)";

#endif // UTILSSEND_H

// ###########################################################
// End
