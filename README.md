ERB-EspWebServer
================

This Program Provides the following for the Esp8266 as an Experimental WebServer:
But CAUTION, some functionallity may not have been throughly tested.  

The code and effort is always changing, check with the GitHub Repositor often, see map at:
https://github.com/wa0uwh/ERB-EspWebServer/network

The purpose of the program is to provide an Experimental Enviroment in which  
to create new functions that can be used in my other planned projects.  

Currently, all Arduino IDE TABS are '.ino' type files, which are compiled as a single blob.  
The structure of this progame could be better, as normally, the TABS should be set up  
as '.h', '.c', or '.cpp' files, but this is easier and quicker for my experimental efforts.  
Future revisions may be properly constructed.  

Current Features:

      o All Web Pages are Dynamically Generated by The Esp8266 Processor.
      o Implemented Full (1460 bytes) WIFI Buffered Transfers.
      o All String Constants are saved in Flash, freeing RAM/Heap, via F(), sF(),
          and FMT() macros.
      o Uses ICACHE_FLASH_ATTR to dynamically load seldom used program functions.
      o Copyright and this Text (Introduction) is Compiled into the Executable as a Web Page.
      o Home Page Dashboard.
      o Help Page.
      o Admin Page, with WiFi Scan and Network Selection.
      o Page Navigator and Resource Links.
      o Raw Data Query.
      o NTP Network Time (Unix Epoch).
      o mDNS Service/Client.
      o Optional Page AutoRefresh.
      o Auto AP Timeout.
      o Implements and uses Html 'meter' function for FreeHeap and Vdd.
      o Implemented SVG 12/24 Hour Clock.
      o Implemented SVG Gauge for FreeHeap.
      o Implemented SVG Gauge for Vdd.
      o Implemented SVG Line Graph for FreeHeap Historical Data, with Start and End of
          Page Markers  and reduced Data Transfer by removing Duplicate Y Data Points.
      o Implemented Data Creation Rate Per Web Page Function.
      o Implemented a Used CPU Seconds Value for Display.
      o Implemented a link to a Visitor Map Display.
      o Implemented Slider Bars, Currently used for Page Refresh Interval.
      o Implemented Code to Serve Raw BINARY Data, for example: the Farm Server Image on Info Page

--
