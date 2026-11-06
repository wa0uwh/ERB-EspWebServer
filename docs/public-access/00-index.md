# Public access methods — index

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106e
# Written: 12:42 PM PST, 6 Nov 2026
# Updated: 3:19 PM PST, 6 Nov 2026

Short notes from the 6 Nov 2026 tests, so a later device can reuse what worked.
The live board that day was ESPHome rev `20261106a` on the ESP32-S3 Super Mini PlainLed48 at `10.20.0.135`.
Boot Slot after the test sequence was `app1 (OTA)`.

Methods that worked end-to-end:

- [01-tailscale-funnel.md](01-tailscale-funnel.md) — public HTTPS via Tailscale Funnel on vbox
- [02-cloudflare-quick-tunnel.md](02-cloudflare-quick-tunnel.md) — temporary `trycloudflare.com` URL
- [03-cinder-rinetd.md](03-cinder-rinetd.md) — one open port on cinder (`http://iot.ebcon.com:8071/`)
- [04-caddy-https.md](04-caddy-https.md) — Caddy on cinder (`https://espws5.ebcon.com/`, no port)
- [06-local-dns-roaming.md](06-local-dns-roaming.md) — Caddy upstream by local DNS name (`esp20.ebcon.com` → `erb-espwebserver-s3.tne`), Lab/Shop roaming plan (testing)

Approaches that did not work, or were abandoned, are in [05-what-did-not-work.md](05-what-did-not-work.md).

For future devices, the lasting path is method 04: one Caddy site block plus an explicit CNAME to `cinder`.

# End
