# Method 2 — Cloudflare Quick Tunnel

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106a
# Written: 12:42 PM PST, 6 Nov 2026

## What it does

Runs `cloudflared` on vbox and gives a random `https://….trycloudflare.com` URL with a real certificate.
No Cloudflare account and no domain move are required.

## Path

Public HTTPS → Cloudflare edge → `cloudflared` on vbox → `http://10.20.0.135:80`.

This does not go through Funnel. Point `cloudflared` straight at the ESP.

## Setup

On vbox (Debian/Ubuntu), install if needed:

```
curl -L --output /tmp/cloudflared.deb https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-linux-amd64.deb
sudo dpkg -i /tmp/cloudflared.deb
```

Start:

```
cloudflared tunnel --url http://10.20.0.135:80
```

It prints a URL. Leave that process running. Ctrl+C stops it, and the next start usually gets a new hostname.

## Public URL that worked

`https://sandwich-acknowledge-still-telephony.trycloudflare.com`

Verified from off-net on 6 Nov 2026: same `20261106a` board, Boot Slot `app1 (OTA)`.

## Limits

- Hostname is random and not memorable.
- Hostname usually changes every restart.
- Fine for a quick test; weak as a standing URL for friends or later projects.
- A named Cloudflare Tunnel with a custom hostname needs a root domain on Cloudflare. Adding `cft.ebcon.com` as a zone was rejected (Cloudflare wants `ebcon.com` or another apex). Moving `ebcon.com` DNS was declined. See [05-what-did-not-work.md](05-what-did-not-work.md).

# End
