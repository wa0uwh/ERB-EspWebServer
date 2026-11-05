<!-- Authors: Grok and Eldon R. Brown - WA0UWH, Rev 20261104 -->
# ERB_EspWebServer ESPHome - notes (newest first)

## 2026-11-04 06:40 PM PST - Rev 20261104i (S3 PlainLed48; LEDs split per variant)
- Eldon: removed the three unwired Red/Green/Blue LED switches from PlainLed48. The Blue one looked out of sync with the heartbeat.
- Split out of common/: common/ERB_Leds_Rgb.yaml (Red/Green/Blue switches; included by NodeMcu + Ws2812) and common/ERB_Heartbeat.yaml (Heartbeat toggle + 2 s WiFi-aware pattern, which drives the switch named by substitution Hb_Target).
  NodeMcu/Ws2812: Hb_Target Blu_LED (same behavior as before). PlainLed48: Hb_Target Heartbeat_LED = internal gpio switch on GPIO48, active-high, ALWAYS_OFF at boot; Heartbeat OFF turns it off.
- erb_ui.js: the LED Control buttons show only for LED entities that exist; the Heartbeat button always shows. Mock (MOCK_NO_LEDS=1) shows only Heartbeat, no JS errors; screens/PlainLed48/*.png. The normal mock still shows all 4 buttons.
- Compile: PlainLed48 RAM 107,435 B (31.4%), Flash 866,819 B (47.2%). Check-only (no new bins): NodeMcu 42.1% / 39.4%, Ws2812 32.5% / 49.3%.
- Baked-in check on the i ota.bin (counts only): WiFi SSID 1, AP password 1, Build_Name 1. The bins are chmod 600 and not in git.

## 2026-11-04 06:30 PM PST - Rev 20261104h (S3 PlainLed48 only)
- AP password now compiled in from secrets.yaml (real fallback hotspot password, chmod 600, never printed or committed).
- Compile: RAM 107,563 B (31.5%), Flash 866,979 B (47.2%). flash/ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104h factory + ota (chmod 600, not in git), Rev_20261104h.txt.

## 2026-11-04 06:25 PM PST - Rev 20261104g (S3 PlainLed48 only)
- PlainLed48 f works on Eldon's HW-747; the red GPIO48 heartbeat is OK.
- WiFi now compiled in from secrets.yaml (real values, chmod 600, never printed or committed). Reason: WiFi saved by improv was lost after each new config, because the config_version_hash changed.
- The g bins contain the WiFi credentials, so they stay out of git (esphome/.gitignore: *.bin, secrets.yaml, .esphome/). Local bins are chmod 600.
- Compile: RAM 107,563 B (31.5%), Flash 866,979 B (47.2%). flash/ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104g factory + ota, Rev_20261104g.txt.
- The NodeMcu and Ws2812 builds were not rebuilt (still 20261104f).

## 2026-11-04 06:00 PM PST - Rev 20261104f (Build Name on the page)
- Eldon: the Ws2812 e build left GPIO48 dark, so he is trying PlainLed48. He asked to see the full bin name on the page.
- Each board file now has substitutions Rev + Build_Name (= the bin base name): ERB_EspWebServer_Esp8266_NodeMcu_20261104f, ERB_EspWebServer_Esp32S3_SuperMini_Ws2812_20261104f, ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104f. The project version comes from ${Rev}.
- common/: text_sensors "Build Name" (static ${Build_Name}) and "Build Time" (App.get_build_time_string; built on the box, so Pacific, e.g. "2026-11-04 17:56:xx -0800").
- erb_ui.js: Build Name in the footer next to the Rev; Info shows Build Name + Build Time. Admin upload shows the Chosen file name and the Running build. It warns (with confirm) when the file's board/variant differs from the running Build_Name, ignoring the _YYYYMMDDx rev and .factory/.ota.bin. Mock test: warning fired for S3 file vs NodeMcu running, then the upload went OK.
- Verified each ota.bin contains its own Build_Name string.
- Compile: NodeMcu RAM 34,472 B (42.1%), Flash 411,569 B (39.4%). Ws2812 RAM 111,171 B (32.5%), Flash 905,003 B (49.3%). PlainLed48 RAM 107,563 B (31.5%), Flash 866,995 B (47.2%).

## 2026-11-04 05:45 PM PST - Rev 20261104e (S3 only; NodeMCU stays 20261104d)
- Eldon: the onboard blue LED blinks about 1/s even with Heartbeat off. That is the battery-charge LED and firmware can't control it (confirmed: fomenko.kyiv.ua/esp32-s3-super-mini-led-rgb-red-blue/ : BLUE = charge, RED = direct on GP48, RGB WS2812 on GP48). Heartbeat was going to IO6 with nothing attached. Eldon said "move it" to GPIO48.
- Ws2812 build: esp32_rmt_led_strip Onboard_Rgb (GPIO48, GRB, 1 LED, internal, 0 s transition). The S3 file uses "!extend Blu_LED" to add on_turn_on (blue, 40%) and on_turn_off, so the common heartbeat and Heartbeat-OFF drive the WS2812 and IO6 together. common/ is unchanged.
- PlainLed48 build: Blu_LED moved to GPIO48 as a plain gpio, inverted: false (active-high guess; the red LED on GP48 shows "ON when output is stable").
- Compile: Ws2812 RAM 111,035 B (32.5%), Flash 904,327 B (49.3%). PlainLed48 RAM 107,427 B (31.4%), Flash 866,335 B (47.2%).
- flash/: ..._Ws2812_20261104e and ..._PlainLed48_20261104e factory + ota bins, Rev_20261104e.txt, ERB_S3_SuperMini_Full_20261104e.yaml.

## 2026-11-04 03:25 PM PST - Rev 20261104d (web login removed)
- Eldon tried guest/guest (we had guest/grest) and asked to remove the login for now. The web_server auth block is commented out in common/ with a re-enable note. The docs no longer mention a login. secrets.yaml.example web_password is now a placeholder.
- Mock: auth only when MOCK_AUTH=1. Re-test without auth: LED, Heartbeat, Reboot, upload and /events all OK, no JS errors.
- Compile: S3 RAM 107,427 B (31.4%), Flash 866,331 B (47.2%). Esp8266 RAM 34,272 B (41.8%), Flash 410,881 B (39.3%).
- flash/: 20261104d factory + ota bins, Rev_20261104d.txt, ERB_S3_SuperMini_Full_20261104d.yaml.
- Note: with no login, anyone on the LAN can reboot or upload firmware to the S3.

## 2026-11-04 03:15 PM PST - Pushed to GitHub (Eldon OK)
- New branch ESPHome_Standalone from origin/master, commit c6c03f0, pushed (no force, no PR). The work went in through a git worktree at /workspace/repos/ERB-EspWebServer_ESPHome, so the parked branches and the main checkout (Fix_Build_Modern_Core) were not touched.
- Excluded via esphome/.gitignore: secrets.yaml, .esphome/, *.bin.
- Heartbeat changed to Eldon's pattern: no WiFi = 1 s on / 1 s off; WiFi = 80/80/80 ms in a 2 s cycle.

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

## 2026-11-04 6:06 PM PST - Diagnosis: S3 bright 1 Hz red blink, no web page after PlainLed48 f upload
- The red LED is on GPIO48 and is our heartbeat. A bright 1 s on/1 s off blink = the "WiFi NOT connected" else-branch (common/ERB_Common.yaml lines 169-172). The app is running; there is no panic or bootloop.
- Cause: the WiFi credentials were entered with improv and saved under App.get_config_version_hash() (esphome wifi_component.cpp:693-695). Any config change (rev, Build_Name, variant) changes the hash, so the new firmware forgets them. secrets.yaml WiFi values are dummy, so there is nothing to fall back on.
- Fallback AP "ERB-EspWebServer-Setup" comes up, but its password is the dummy, which Eldon does not know.
- Fix now: USB to web.esphome.io > Connect > Configure WiFi (improv); no reflash needed. Permanent fix: real WiFi in secrets.yaml and/or a known AP password.

## 2026-11-04 6:37 PM PST - Rev 20261104j: footer shows full Build Name / upload file name
- Rev f claimed the footer showed Build Name; it never did (only "Rev " + Rev). Fixed: footer "WA0UWH - ERB-EspWebServer - Rev 20261104j - <Build Name>" (span ftBuild, filled in Refresh(), '--' if empty).
- Info tab: kept Build Name row, added "Upload File: <Build Name>.ota.bin" (id infoFile).
- Mock checked (no JS errors), screens/PlainLed48/ refreshed.
- flash/ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104j.ota.bin (867024 B), .factory.bin (932560 B), Rev_20261104j.txt. Credentials baked in from secrets.yaml. NodeMCU + Ws2812 compile OK (no bins).
- Note: PlainLed48 and Ws2812 share build dir erb-espwebserver-s3; compile PlainLed48 LAST before copying bins.

## 2026-11-04 7:39 PM PST - Rev 20261104k: Home firmware line, developer footer, heap sparkline
- Home: under LED Control/Heartbeat, small line "Firmware: <Build Name>.factory.bin / .ota.bin" (id homeFw, Refresh(), '--' if empty).
- Footer: "WA0UWH - ERB-EspWebServer" then "Firmware <Build Name> | Built <Build Time> | Rev | Up d hh:mm:ss | Heap kB" (ftBuild/ftTime/ftUp/ftHeap, live in Refresh()).
- Above footer: 300x40 SVG heap sparkline (Spark[], last 300 Free Heap samples ~10 min, browser memory only), min/max/now text, blue min marker, uptime text beside it. Home history graph unchanged.
- Mock checked (no JS errors), screens/PlainLed48/ refreshed. Full compile log flash/Build_20261104k.log (no warnings, no secrets).
- flash/ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261104k.factory.bin (933232 B), .ota.bin (867696 B), Rev_20261104k.txt. Credentials baked in. NodeMCU + S3 base compile OK (no bins). PlainLed48 compiled last.

## 2026-11-04 7:43 PM PST - Rev 20261104l: on-device heap history (flash ring, served locally)
- components/erb_heap_log (external_components, local) + common/ERB_HeapLog.yaml (PlainLed48 only; esp-idf). Every 60 s (set_interval, main loop) stores 8 B {epoch s if SNTP valid else uptime s, free/16, boot&0xF<<12 | minfree/256} in a 2560-record ring (~42.7 h) = 40 NVS blobs x 64 records, namespace "erbheap", header {magic, head, count, boot}.
- Storage: existing NVS partition (0x70000 = 448 KB). Partition table UNCHANGED (first 64 KB of factory.bin identical to k) - OTA from k is fine.
- Wear: ~1 x 512 B blob + 12 B header per minute ~ 0.9 MB/day written into NVS, which rotates over ~110 4 KB pages -> ~2 erases/page/day -> 100k-cycle flash ~ 100+ years.
- Safe: any NVS open/write failure -> ESP_LOGW and disable/pause; never blocks boot. LATE setup priority.
- GET http://erb-espwebserver-s3.local/heap_history.csv -> "stamp,free,minfree,boot" oldest first, chunked (no big RAM buffer).
- UI: second 300x40 sparkline "device" long trend (blue, green dashed = reboot, low-water text), fetched on load + every 5 min; Admin "Clear heap history" (confirm) -> button/Clear Heap History/press.
- Mock serves fake 600-min, 2-boot history. Log flash/Build_20261104l.log: 0 warnings. Bins: _20261104l.factory.bin 935792 B, .ota.bin 870256 B.
