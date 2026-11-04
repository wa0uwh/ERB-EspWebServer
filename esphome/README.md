<!-- Authors: Grok and Eldon R. Brown - WA0UWH, Rev 20261104d -->
# ERB_EspWebServer (ESPHome, standalone)

Standalone ESPHome rework of WA0UWH's early ERB-EspWebServer (Arduino IDE, 2015). No Home Assistant: no `api:` block; the device serves its own page (`web_server` v3) at `http://erb-espwebserver.local/`.

Files: `common/ERB_Common.yaml` (shared), `ERB_EspWebServer_Esp8266.yaml` (nodemcuv2), `ERB_EspWebServer_Esp32S3_SuperMini.yaml` (HW-747 S3 Super Mini, 4 MB, LEDs IO4/5/6, input IO7, logs on USB), UI `www/erb_ui.js`.

Board (Esp8266): `nodemcuv2` (original drives LEDs on GPIO0/2/4, i.e. ESP-12 class). Alternative: `esp01_1m` for an ESP-01 (remove Blue LED / GPIO4 and the GPIO5 input; only GPIO0/2 are broken out).

| Original feature | ESPHome equivalent | Lost / changed |
|---|---|---|
| WiFi connect, Admin WiFi scan & select | `wifi` + fallback AP + `captive_portal`; `wifi_info` (IP, SSID, BSSID, MAC, scan results), `wifi_signal` | Network choice made in the captive portal, not a custom Admin page |
| Auto AP timeout | fallback AP comes up only when STA fails | Not configurable the same way |
| mDNS service/client | `mdns` | Client browsing of other hosts |
| NTP time, SVG clock | `sntp` + "Clock" text; SVG analog clock in erb_ui.js, Navigator clock | Drawn by browser JS |
| FreeHeap meter, line graph | `debug` sensors; SVG instrument + 5-60 min line graph in erb_ui.js | History kept in browser only |
| Vdd meter | `adc` VCC (8266); S3: Chip Temp meter instead | S3 has no Vdd |
| LED / I/O control (Red/Grn/Blu), heartbeat | `gpio` switches (active-low), `gpio` binary_sensor example, Heartbeat switch (Blue LED, WiFi-aware) | none |
| Info/version, CPU, uptime | `version`, `debug` device info + reset reason, `uptime` | CPU-seconds, data-rate stats |
| Reboot | `restart` and `safe_mode` buttons | none |
| Home/Help/Info/Admin pages, Navigator | erb_ui.js tabs + Navigator + "All" entity tab; live via /events | Visitor Map, Farm image, demo graphics, sliders (not needed) |

Web login: none for now (auth block commented in common/, can be re-enabled). Custom UI: `www/erb_ui.js` is compiled into flash (`js_include`, `js_url: ""`), so the page needs no internet. Test without hardware: `node mock/Mock_Server.js` then open http://127.0.0.1:8099/.

## Build / flash
1. `cp secrets.yaml.example secrets.yaml` and fill in.
2. `esphome compile ERB_EspWebServer_Esp8266.yaml` (or `..._Esp32S3_SuperMini.yaml`)
3. FIRST flash must be USB serial: `esphome run ERB_EspWebServer_Esp8266.yaml --device /dev/ttyUSB0` (ESP-01 needs GPIO0 to GND at power-up for flashing).
4. Later flashes over the air: `esphome run ERB_EspWebServer_Esp8266.yaml --device erb-espwebserver.local`.
5. If WiFi fails, join AP "ERB-EspWebServer-Setup" and pick a network in the captive portal.

Note: GPIO0/GPIO2 are boot-strap pins; LEDs to 3.3V (active-low) are fine, don't pull them low at boot.

Tested 20261104: config + compile pass for both. Esp8266 RAM 41.6% / Flash 39.4%; S3 RAM 29.9% / Flash 46.4%. UI tested in headless Chrome against a mock. Not tested on hardware.
<!-- End -->
