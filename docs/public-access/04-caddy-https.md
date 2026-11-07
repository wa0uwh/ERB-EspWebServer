# Method 4 — Caddy HTTPS on cinder (recommended)

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106f
# Written: 1:20 PM PST, 6 Nov 2026
# Updated: 3:33 PM PST, 6 Nov 2026

## What it does

Caddy on cinder terminates HTTPS on port 443 and reverse-proxies to the ESP.
Friends use a short name with no port, and a real Let’s Encrypt certificate.
`ebcon.com` stays at Network Solutions; only one explicit CNAME is added.

## Path

Public HTTPS 443 (and HTTP 80 for the certificate check and redirect) → Caddy on cinder → `http://10.20.0.135:80`.

## DNS

`ebcon.com` has a catchall:

```
CNAME  *  dc01.ebcon.com
```

`dc01` goes to the home IP `73.97.143.6`. Any name without its own record follows that catchall.
Let’s Encrypt will try that home address and time out unless you add an explicit record to cinder.

Working record used on 6 Nov 2026:

- `espws5.ebcon.com` CNAME → `cinder.ebcon.com` (then the existing `cinder` → `dc02` → `dy02.mooo.com` → `138.197.239.186` chain)

## Caddy

Install Caddy from the official Debian repo (see https://caddyserver.com/docs/install#debian-ubuntu-raspbian), then `/etc/caddy/Caddyfile`:

```
espws5.ebcon.com {
    reverse_proxy http://10.20.0.135:80 {
        header_up -Cookie
    }
}
```

`header_up -Cookie` drops the browser Cookie header before Caddy forwards the request to the ESP.

What was actually observed (6 Nov 2026): the normal local browser got HTTP `431` with body
`Header fields are too long`. A test with a large (~2000-byte) Cookie header reproduced the same 431,
while a short request returned 200. An incognito window (no old cookies) loaded the page. After adding
`header_up -Cookie`, Eldon confirmed the normal browser worked. The likely cause is the ESP HTTP
server's small request-header buffer versus many `*.ebcon.com` cookies; the exact buffer size was not
measured. The board does not need cookies (web login is off).

```
sudo caddy validate --config /etc/caddy/Caddyfile && sudo systemctl reload caddy
sudo ss -lntp | grep -E ':80|:443'
```

Caddy should be listening on `*:80` and `*:443`. rinetd can keep `0.0.0.0:8071` for method 03.

## Public URL that worked

`https://espws5.ebcon.com/`

Let’s Encrypt certificate for `espws5.ebcon.com`, issued 6 Nov 2026 at 11:31 AM PST, valid through 4 Feb 2027 at 11:31 AM PST.
Verified from off-net: ESPHome page, Build Name `ERB_EspWebServer_Esp32S3_SuperMini_PlainLed48_20261106a`, Boot Slot `app1 (OTA)`.

## Lesson from the first cert attempt

The Caddyfile first said `espws1.ebcon.com` while only `espws5` pointed at cinder.
Let’s Encrypt tried `espws1`, followed the catchall to `73.97.143.6`, and logged:

`Timeout during connect (likely firewall problem)` against `73.97.143.6`.

The Caddyfile hostname and the explicit CNAME must match, and that CNAME must point at cinder, not the catchall.

## Second test name: esp2 abandoned, esp20 used

`esp2.ebcon.com` got an explicit CNAME to `cinder`, and `dig @1.1.1.1` already showed the new chain,
but Let’s Encrypt still reached the old home IP `73.97.143.6` and timed out (http-01 and tls-alpn-01).
`esp2` had a very old, long-lived record, so a cached old answer is the likely reason.
No exact cache-expiry time is guaranteed; Caddy retries certificates automatically, so repeated reloads are not needed.
Eldon switched to the fresh name `esp20`:

- `esp20.ebcon.com` CNAME → `cinder.ebcon.com` → `dc02.ebcon.com` → `dy02.mooo.com` → `138.197.239.186` (confirmed with `dig +short @1.1.1.1 esp20.ebcon.com`)

Caddy then obtained the certificate and HTTPS worked. Lesson: for a test, a brand-new name avoids stale caches.

Later the same day (3:25 PM PST) Eldon confirmed `esp2.ebcon.com` working too. That fits old DNS caches
expiring, but the cause was not independently proven. Both names are now aliases in one site block
(see 06); each needs its own explicit CNAME to `cinder`.

## Upstream by local DNS name (no hard-coded IP)

`esp20.ebcon.com` proxies to the device name `erb-espwebserver-s3.tne`, resolved through the router DNS
at `10.20.0.1`. The internal name does not need to match the public certificate, because Caddy talks
plain HTTP to the ESP. Working, user-confirmed block and details are in
[06-local-dns-roaming.md](06-local-dns-roaming.md).

## Reading the reload logs

- `Caddyfile input is not formatted` is cosmetic (`caddy fmt --overwrite` fixes it).
- `servers shutting down with eternal grace period` after `caddy validate` is the temporary validate process; normal.
- The same line during a reload is the old server config stopping; normal.

## More devices

For each new board, add one site block and one explicit CNAME to `cinder`:

```
espws6.ebcon.com {
    reverse_proxy http://10.20.0.136:80 {
        header_up -Cookie
    }
}
```

Then `sudo systemctl reload caddy`. Use that device’s LAN or tailnet address in `reverse_proxy`.
Ports 80 and 443 are already open; no new public port is required.
Give Let’s Encrypt a minute after the CNAME is live.

Scanner 502s in the Caddy log (`/config.json`, `/telescope/requests`, and similar) are internet probes, not a reason to create those files on the ESP.

# End
