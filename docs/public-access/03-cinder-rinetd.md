# Method 3 — One public port on cinder (rinetd)

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106a
# Written: 12:42 PM PST, 6 Nov 2026

## What it does

Uses cinder, a cloud VM with a public IP that is also on the tailnet, as a simple TCP forwarder.
Testers hit `hostname:8071` over plain HTTP. No TLS.

## Path

Public TCP 8071 → rinetd on cinder → `10.20.0.135:80`.

cinder can already `curl http://10.20.0.135/`, so no hop through vbox is required.

## DNS

- `cinder.ebcon.com` → `dc02.ebcon.com` → `dy02.mooo.com` → `138.197.239.186`
- `dc02` is the DataCenter02 alias for cinder
- `iot.ebcon.com` was added as a CNAME to `cinder` (overrides the `*` catchall)

## Setup

On cinder, `/etc/rinetd.conf`:

```
0.0.0.0     8071        10.20.0.135     80
logfile /var/log/rinetd.log
```

Watch the destination address: `10.20.10.135` (a typo) made rinetd accept the TCP connection and then close it with an empty reply, even on localhost.
After correcting the file, reload:

```
sudo kill -HUP $(pidof rinetd)
curl -sv --max-time 5 http://127.0.0.1:8071/
```

Open port 8071 on cinder’s cloud firewall. rinetd should listen on `0.0.0.0:8071`, not only `127.0.0.1`.

## Public URLs that worked

- `http://cinder.ebcon.com:8071/`
- `http://iot.ebcon.com:8071/`

Verified from off-net on 6 Nov 2026: ESPHome HTML and the usual Build Name / Boot Slot sensors.

## Limits

- URL includes `:8071`.
- Traffic is plain HTTP.
- Each extra device can get another port (8072, …) or, better, share 443 via Caddy (method 04).
- Do not stack this in front of Funnel; it replaces Funnel for this path.

# End
