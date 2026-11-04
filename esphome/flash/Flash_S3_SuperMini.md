<!-- Authors: Grok and Eldon R. Brown - WA0UWH, Rev 20261104d -->
# Flash the ESP32-S3 Super Mini (first time, no ESPHome install)

You need: Chrome or Brave on a computer, a USB-C **data** cable, and the file
`ERB_EspWebServer_Esp32S3_SuperMini_20261104d.factory.bin`.

1. Plug the Super Mini into the computer with the USB-C cable.
2. Open **https://web.esphome.io** in Chrome or Brave.
3. Click **Connect** and pick the port (for example "USB JTAG/serial debug unit" or a COM / ttyACM port).
   - No port listed? Hold the **BOOT** button, tap **RESET** (RST), then let go of BOOT. Click Connect again.
4. Click **Install**, choose the `.factory.bin` file, and confirm. Wait for "Installation complete" (about 1 minute).
   - If you used BOOT/RESET in step 3, tap **RESET** once afterward, then Connect again.
5. When asked, enter your **WiFi name and password** and click Connect.
6. Open **http://erb-espwebserver-s3.local/** (or the IP address the web page shows) in your browser.

If the board can't join WiFi, it makes its own hotspot **ERB-EspWebServer-Setup**.
Join it from a phone, and a setup page opens where you can pick your WiFi network.

Blue LED (IO6) heartbeat: slow 1 s on / 1 s off = no WiFi yet; quick double-blink every 2 s = WiFi connected.
Turn it off with the **Heartbeat** button on Home.

## Updating later (no USB) - Rev 20261104b and newer (current 20261104d)
1. Open http://erb-espwebserver-s3.local/ and click **Admin**.
2. Under **Firmware update**, choose the new `...ota.bin` file and click **Upload**, then OK.
3. Wait about 30 seconds while it reboots, then reload the page. Check the Rev in the footer.

Boards still on Rev 20261104 have no Upload button yet. Use the USB steps above once with the 20261104b `.factory.bin`.
<!-- End -->
