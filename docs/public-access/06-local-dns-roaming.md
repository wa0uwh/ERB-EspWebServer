# Method 6 — Caddy upstream by local DNS name, and roaming plan

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106f
# Written: 3:19 PM PST, 6 Nov 2026
# Updated: 3:33 PM PST, 6 Nov 2026

## Why

No hard-coded device IPs in the Caddyfile, especially for roving devices.

## Current working config (Eldon, "for the record", 3:32 PM PST, 6 Nov 2026)

```
# Testing DNS Lookup (with Aliases) from LAB and Shop for Actual Device Name
esp2.ebcon.com, esp20.ebcon.com {
    reverse_proxy {
        dynamic a erb-espwebserver-s3.tne 80 {
            resolvers   10.20.0.1:53    10.10.10.1:53
            versions ipv4
            refresh 30s
        }
        header_up -Cookie
    }
}
```

- Lab DNS `10.20.0.1:53`, Shop DNS `10.10.10.1:53`.
- Site names (aliases) are comma-separated; resolvers are whitespace-separated.
- Eldon confirmed `esp2` working at 3:25 PM PST and the alias config ("Yes, that worked") at 3:29 PM PST.

## Earlier single-name block (also worked)

```
esp20.ebcon.com {
    reverse_proxy {
        dynamic a erb-espwebserver-s3.tne 80 {
            resolvers 10.20.0.1:53
            versions ipv4
            refresh 30s
        }
        header_up -Cookie
    }
}
```

Public DNS: both `esp2` and `esp20` need an explicit CNAME → `cinder` (see [04-caddy-https.md](04-caddy-https.md)).
Validate and reload on cinder:

```
sudo caddy validate --config /etc/caddy/Caddyfile && sudo systemctl reload caddy
```

## What failed first, and the tests

First try used `transport http { resolvers 10.20.0.1:53 }` with `reverse_proxy erb-espwebserver-s3.tne:80`.
Caddy logged `dial tcp: lookup erb-espwebserver-s3.tne: i/o timeout` (502 after ~3 s).

Tests on cinder:

- `dig @10.20.0.1 erb-espwebserver-s3.tne A` → `10.20.0.135`, 32 ms
- `sudo -u caddy dig @10.20.0.1 erb-espwebserver-s3.tne A` → `10.20.0.135`, 37 ms
- `dig @10.20.0.1 erb-espwebserver-s3.tne AAAA` → timed out
- `curl http://10.20.0.135/` → HTTP 200, ESP page

So IPv4 DNS and the ESP were fine; the unanswered IPv6 (AAAA) query was the problem.
`dynamic a` with `versions ipv4` asks only for A records.

## Multiple DNS servers

Separate with whitespace, not commas (as in the current config above: Lab then Shop).
Every listed resolver must know the `.tne` names (a public resolver will not).
Do not assume ordered failover, or that Caddy reconciles conflicting or stale answers between resolvers.

## Lab/Shop roaming plan (TESTING, not proven — no movement test done yet)

- cinder reaches both the Lab and Shop subnets over Tailscale.
- Each site's router has its own DNS server; mDNS is forwarded between the sites.
- ESPs are, or can be, configured to expire their old DNS/mDNS names on reboot and request new ones.
- Goal: rovers self-heal in DNS. That needs DNS updates plus a reachable route to the new address; Caddy only does the lookup.

Movement test to run:

1. Move a rover from Lab to Shop while Caddy is running.
2. Check the old record disappears and the new one is visible through both resolvers.
3. Check `esp20.ebcon.com` recovers without a Caddy reload (refresh is 30 s).
4. Watch for conflicting or stale answers when more than one resolver is listed.

# End
