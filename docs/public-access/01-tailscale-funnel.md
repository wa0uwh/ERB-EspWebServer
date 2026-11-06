# Method 1 — Tailscale Funnel on vbox

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106a
# Written: 12:42 PM PST, 6 Nov 2026

## What it does

Exposes the ESP web UI to the public internet through Tailscale Funnel, without opening a port on the home router.
The public name is the Funnel hostname, not an `ebcon.com` name.

## Path

Public HTTPS port 443 → Tailscale Funnel edge → vbox port 8071 → rinetd on vbox → `10.20.0.135:80` (the ESP).

## Machines

- Funnel host: `vbox` on the tailnet `vbox.tailb588b8.ts.net` (tailnet address `100.96.10.13`)
- ESP: `10.20.0.135` on the LAN (also reachable from some tailnet nodes)

## Setup

On vbox, rinetd:

```
0.0.0.0  8071  10.20.0.135  80
```

Start Funnel in the background so it survives Ctrl+C:

```
sudo tailscale funnel --bg 8071
```

Check or stop:

```
sudo tailscale funnel status
sudo tailscale funnel reset
```

A foreground `tailscale funnel 8071` dies when you press Ctrl+C. That is why the first attempt looked like Funnel was set up but was not reachable.

## Public URL that worked

`https://vbox.tailb588b8.ts.net/`

Funnel’s public listener is 443 (also 8443 or 10000). It is not 8071 on the public side. `8071` is only the port on vbox that Funnel forwards to.

Verified from off-tailnet on 6 Nov 2026: ESPHome HTML, Build Name `…_20261106a`, Boot Slot `app1 (OTA)`.

## Limits

- The public URL is the long `ts.net` name.
- An A record to `100.96.10.13` is tailnet-only; strangers cannot use it.
- A CNAME such as `espws4.ebcon.com` → `vbox.tailb588b8.ts.net` resolves, but HTTPS fails because the Funnel certificate is only for the `ts.net` name (and the CNAME reveals the tailnet name).
- Keep Funnel if you want this URL for testers; it does not replace a short `ebcon.com` HTTPS name.

# End
