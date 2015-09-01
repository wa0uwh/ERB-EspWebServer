// IO_CTL

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
//
void
redLed(boolean state)
{
  gRedLedState = state;
  digitalWrite(gRedLED, gRedLedState);
  Serial.println ( sF("RedLED") + String(gRedLedState == ON ? "ON" : "OFF") );
}

// ###########################################################
//////////////////////////////////////////////////////////////
//
//
void
redLed_toggle()
{
  gRedLedState = !gRedLedState;
  redLed(gRedLedState);
}


// ###########################################################
// End
