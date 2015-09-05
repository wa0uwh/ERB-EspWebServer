// IO_CTL

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
