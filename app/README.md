# 🔒 GLYPH for Android

The companion app: a detailed world map, route planning, team chat and every setting of your GLYPH,
on your phone.

> **The source code is private.** The app is free to download and use. See [LICENSE](../LICENSE).

**Latest version: 1.3** · [What's new](../CHANGELOG.md) ·
**[⬇️ Download GLYPH-1.3.0.apk](https://github.com/Mqr1oo/GLYPH/releases/latest)** · Google Play: coming soon

<p align="center">
  <img src="../docs/images/app/route.jpg" width="24%" alt="Route planning on the detailed dark map"/>
  <img src="../docs/images/app/comms.jpg" width="24%" alt="Team chat"/>
  <img src="../docs/images/app/status.jpg" width="24%" alt="Live readings"/>
  <img src="../docs/images/app/control.jpg" width="24%" alt="Settings"/>
</p>

---

## Install

1. Download `GLYPH-x.y.z.apk` from [Releases](https://github.com/Mqr1oo/GLYPH/releases/latest) on your phone.
2. Open it. If Android asks, allow your browser to install apps.
3. Open GLYPH. The first screen installs the firmware over USB-C, with the access code that came with
   your GLYPH. Then tap the GLYPH logo to pair.

You need **Android 7 or newer** with Bluetooth. After installing, the app **updates itself**: a
new version downloads in the background and starts the next time you open the app.

## The five tabs

| Tab | What it does |
|---|---|
| 📊 **Status** | Time, satellites, temperature, humidity, GLYPH's position (tap to copy), recording, hardware check, firmware and memory card |
| 💬 **Comms** | Public or Secure chat over LoRa, quick replies, search, and a delivery status on every message |
| 🗺️ **Routes** | The detailed dark map or satellite. Plan a route on foot, by bike or by car, or draw it. Send it to GLYPH's memory or its card. Save map areas or a band along your route for offline use |
| 📁 **Files** | Tracks recorded on the phone and on GLYPH, KML import and export, books for the e-ink reader |
| ⚙️ **Control** | Name, language, radio band, time zone, pairing, team channel, power mode, alerts, range check, diagnostics, firmware |

## Permissions

| Permission | Why |
|---|---|
| **Nearby devices** (Bluetooth) | To find your GLYPH and talk to it |
| **Notifications** | New messages, SOS alerts and the live trip notification |
| **USB device** (asked when you plug GLYPH in) | To install firmware over the cable |

The app does **not** track your phone's location: the position it shows comes from GLYPH's own GPS.
Older Android versions ask for location access only because they require it to scan for Bluetooth
devices.

## With and without internet

Everything works offline: chat, team radar, routes you have planned, recorded tracks, settings, and
the map areas you saved. Internet is used only for:

- the detailed map of places you have not saved, and satellite images;
- calculating a new route, and finding a street, peak or hut (towns and villages are built in);
- app and firmware updates.

No account, no ads, no analytics. Read the [privacy policy](../PRIVACY.md).

## Map data

Map data © [OpenStreetMap](https://www.openstreetmap.org/copyright) contributors, served by
[OpenFreeMap](https://openfreemap.org) (OpenMapTiles schema) and [Protomaps](https://protomaps.com).
Routes by [FOSSGIS](https://routing.openstreetmap.de) and [OSRM](https://project-osrm.org).
Places from [GeoNames](https://www.geonames.org) (CC BY 4.0).
