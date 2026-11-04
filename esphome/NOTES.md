<!-- Authors: Grok and Eldon R. Brown - WA0UWH, Rev 20261104 -->
# ERB_EspWebServer ESPHome - notes (newest first)

## 2026-11-04 03:10 PM PST - Rev 20261104c
- Web auth is back on, user guest / password grest (Eldon: example project, OK in secrets.yaml.example + docs). It covers the page, /events, REST POSTs and /update.
- Mock: Mock_Server.js now enforces basic auth (401 without it). Shoot_Tabs.js logs in, and these all passed through auth: LED toggle, Heartbeat toggle, Reboot POST, the OTA upload (multipart /update) and the SSE stream. No-auth /events -> 401.
- Heartbeat template switch (RESTORE_DEFAULT_ON) + 2 s interval in common/ drives the Blue LED. Before WiFi it blinks 1 s on / 1 s off. With wifi.connected it does 80 on / 80 off / 80 on, then off. Switching it off turns Blue off and gives manual control back. Heartbeat button on Home.
- Compile: S3 RAM 107,435 B (31.4%), Flash 867,563 B (47.3%). Esp8266 RAM 34,468 B (42.1%), Flash 414,437 B (39.7%).
- flash/: 20261104c factory + ota bins, Rev_20261104c.txt (replaces the b marker), ERB_S3_SuperMini_Full_20261104c.yaml (review copy).

## 2026-11-04 02:56 PM PST - Rev 20261104b (S3)
- Eldon's S3 flashed OK. Boot log: ESP32-S3 QFN56 rev v0.2, 4MB embedded flash (XMC), 2MB embedded quad PSRAM (AP_3v3). So psram: mode quad 80MHz is enabled, and the "PSRAM Free" debug sensor is on (shows on the Info and All tabs).
- Chip Temp gauge range is now 20-100 C (it read 62 C).
- Rev 20261104b in the S3 project version, the erb_ui.js footer and the Navigator rev line.
- Added ota: platform web_server on the S3, plus a Firmware update form on the Admin tab (POST /update with the .ota.bin). This lets him update without USB from 20261104b on. The 8266 build has no web OTA, so its form will report a failure. Risk: the page has no login, so anyone on the LAN can upload firmware. Re-enable web_server auth if that matters.
- Compile: S3 RAM 107,075 B (31.3%), Flash 863,927 B (47.1%). Esp8266 RAM 33,852 B (41.3%), Flash 408,493 B (39.1%).
- flash/: 20261104b.factory.bin (929,584 B), 20261104b.ota.bin (864,048 B), Rev_20261104b.txt. The old 20261104.factory.bin is kept.
- The upgrade from 20261104 must go over USB (web.esphome.io), because that build has no upload form. The OTA password option needs ESPHome installed and the dummy OTA password.

## 2026-11-04 02:45 PM PST - Pacific time + S3 novice flash kit
- Clocks: sntp timezone POSIX "PST8PDT,M3.2.0,M11.1.0" (America/Los_Angeles). erb_ui.js header + SVG clock now always show Pacific time with PST/PDT label (Intl, timeZone America/Los_Angeles), regardless of the viewer's PC zone. Re-shot screens/1_Home.png.
- S3: added improv_serial: (on USB_SERIAL_JTAG) so web.esphome.io asks for WiFi after flashing; WiFi secrets can stay dummy. Fallback AP + captive portal kept.
- web_server auth commented out (no login by default) so no unknown password is needed. OTA password still in build (only needed for later WiFi updates via ESPHome). Fallback AP password still from secrets.yaml (dummy) - needs a real value Eldon knows, or AP made open.
- flash/ERB_EspWebServer_Esp32S3_SuperMini_20261104.factory.bin + Rev_20261104.txt + Flash_S3_SuperMini.md (novice web.esphome.io steps). Device URL: http://erb-espwebserver-s3.local/
- Compile: S3 RAM 102,407 B (30.0%), Flash 856,423 B (46.7%). Esp8266 RAM 33,852 B (41.3%), Flash 408,173 B (39.1%).

## 2026-11-04 02:35 PM PST - Custom ERB UI + ESP32-S3 Super Mini
- Split config: common/ERB_Common.yaml (shared UI + entities) + ERB_EspWebServer_Esp8266.yaml (nodemcuv2) + ERB_EspWebServer_Esp32S3_SuperMini.yaml. Old single file + erb_gauges.js moved to old/.
- New www/erb_ui.js (js_include, js_url: "" so NOTHING loads from the internet). Replaces the stock page with Home/Help/Admin/Info/All tabs + 2015-style Navigator.
- ESPHome 2026.10 facts used (checked in venv source web_server.cpp/__init__.py): SSE ids are "<domain>/<Entity Name>" (name-based, not object_id); REST POST /switch/Red%20LED/toggle, /button/Restart/press. "local: true" would serve the stock bundle and IGNORE js_include, so not used.
- Compile: Esp8266 RAM 34,052 B (41.6%), Flash 411,517 B (39.4%). S3 RAM 102,311 B (29.9%), Flash 851,451 B (46.4% of 1.75 MB app partition).
- Browser test: mock/Mock_Server.js + mock/Shoot_Tabs.js (headless Chrome), no JS errors, LED toggle and Reboot POSTs correct. Screens: screens/1_Home..5_All.png (Esp8266 entity set).
- S3 board (HW-747 V0.0.2 Super Mini) assumptions: ESP32-S3FH4R2, 4 MB flash, 2 MB QUAD PSRAM (left commented until confirmed), WS2812 on GPIO48 (optional light, commented), logs on native USB (USB_SERIAL_JTAG). LEDs IO4/5/6, input IO7 - all on headers, not strap pins.
  Sources: https://www.espboards.dev/esp32/esp32-s3-super-mini/ , https://esp32.co.uk/esp32-s3-supermini-pinout-safe-gpios-usb-psram/ , https://devices.esphome.io/devices/tenstar-robot-esp32-s3-supermini/
- Original look inventory (from Main/*.ino): background #ddddee; centered H1 "<name> - <page>" + small "ERB's Esp8266 Experimental Web Server"; Navigator table: left "| Home | Help | Admin | Info |", center clock, right MFR/Forum/BBS/TheBOOK/IDE/GitHub links; under it "FreeHeap kB - Hits - Rev". SVG clock/instrument: radial gradient #DDDDDD->#111122, black rim, white ticks, red/black hub, #44FF00/#FF4400 accents. Free Heap line graph: lightblue rect, white grid, red trace. Home: LED form button "Red LED is ON -> [OFF]", random line/bar demos, AutoUpdate + slider refresh. Help: <pre> lists of Pages, Modes (/t/redled etc.), Query (/q/ssid, rssi, mdns, ipa, mac, gateway...). Admin: WiFi scan/select, reboot. Info: version, CPU, uptime, copyright. Extras: VisitorMap, Farm01 jpg.
- Still missing vs original: Visitor Map, farm image, Bar/random demo graphics, hit counter is per-browser (not device), verbose mode, gateway/netmask/port queries, WiFi select on Admin (captive portal instead), heap history is browser-side (lost on reload).

## 2026-11-03 02:57 PM PST
- Created standalone ESPHome port: ERB_EspWebServer.yaml (nodemcuv2, alt esp01_1m), no api:, web_server v3 + optional www/erb_gauges.js (SVG clock + heap gauge).
- ESPHome 2026.10.2 in /workspace/tools/esphome-venv. config: valid. compile: success (RAM 34,108 B 41.6%, Flash 407,733 B 39.0%) with dummy secrets. Not on hardware; JS add-on not browser-tested.
<!-- End -->

## 2026-11-04 2:52 PM PST
- Eldon flashed the S3 Super Mini (HW-747) using web.esphome.io, and it now runs live on his network. Pacific clock, heap gauge (255 kB), Chip Temp (62 C) and the history graph all work. Chip: ESP32-S3 QFN56 rev v0.2, 4MB flash (XMC), 2MB quad PSRAM (AP_3v3).
- Next build: enable PSRAM (quad), widen the Chip Temp gauge past 80 C. The hotspot password is still the dummy value.
