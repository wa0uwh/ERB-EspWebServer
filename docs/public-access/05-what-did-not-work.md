# What did not work, or was set aside

# Authors: Grok and Eldon R. Brown - WA0UWH
# Rev: 20261106a
# Written: 12:42 PM PST, 6 Nov 2026

## A record to the tailnet address

`espws1.ebcon.com` (and similar) as an A record to `100.96.10.13` only works on the tailnet.
Off-net clients cannot reach Tailscale CGNAT `100.64.0.0/10`.
`espws1.ebcon.com:8071` was not a public Funnel URL.

## CNAME to the Funnel `ts.net` name

`espws4.ebcon.com` CNAME → `vbox.tailb588b8.ts.net` resolved publicly, and HTTP redirected to HTTPS, but TLS failed: the Funnel certificate SAN is only `vbox.tailb588b8.ts.net`.
That CNAME also publishes the tailnet name.

## Cloudflare “Add a site” for `cft.ebcon.com`

Cloudflare’s add-domain form rejects a subdomain (`Provide the root domain … not subdomain.example.com`).
Delegating `cft.ebcon.com` with NS records was the idea that would have avoided moving `ebcon.com`; this form will not start that way.
Partial/CNAME setup (keep DNS at Network Solutions, proxy only one name) is a Cloudflare Business/Enterprise feature, not free.

## Moving `ebcon.com` to Cloudflare

The wizard will accept `ebcon.com` and ask to change nameservers.
That was declined because of the many existing records at Network Solutions.
To abandon a started onboarding: Cloudflare home → click `ebcon.com` → Overview → Remove site. That does not change Network Solutions.

## Buying a new apex (`ebcon.dev`, `ebcon.io`, …)

Would give Cloudflare a root domain without moving `ebcon.com`, but it is not free.
`ebcon.io` looked unused in DNS, though `.io` is a relatively expensive TLD.
Existing GoDaddy domains such as `wa0uwh.net` were not moved either.

## Telnet / extra Cloudflare wrapping

Cloudflare Tunnel does not speak Tailscale; `cloudflared` just needs an HTTP URL it can reach.
Do not stack Cloudflare in front of Funnel.
A raw Telnet/TCP tunnel would require `cloudflared` on each client; it was not used.

# End
