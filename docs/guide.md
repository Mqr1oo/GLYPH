# 📖 GLYPH user guide

Everything you need to get from the box to the trail.

- [First start](#first-start)
- [The device](#the-device)
- [Messages](#messages)
- [Routes and maps](#routes-and-maps)
- [Recording a track](#recording-a-track)
- [SOS](#sos)
- [Battery and power modes](#battery-and-power-modes)
- [Updates](#updates)
- [Troubleshooting](#troubleshooting)

---

## First start

1. **Install the app**: download `GLYPH-x.y.z.apk` from
   [Releases](https://github.com/Mqr1oo/GLYPH/releases/latest) and open it on your Android phone.
2. **Install the firmware**: the app opens on the *Install firmware* screen. Plug GLYPH into the
   phone with a USB-C data cable, enter the **access code** that came with your GLYPH and tap
   **Install**. GLYPH restarts after about a minute.
3. **Set it up**: unplug GLYPH and tap the GLYPH logo at the top of the app. Android asks for a
   **6-digit code**: type the one shown on GLYPH's screen. A new GLYPH then asks, in the app, for a
   name, language, radio band and time zone.
4. **Take the tour**: a short guide points at every part of the app. You can replay it from
   **Control › App › Show me around again**.

## The device

GLYPH has three buttons, **A**, **B** and **C**. The menus:

| Menu | What it shows |
|---|---|
| **Tactical map** | Your live track, distance and heading, and the route sent from the app. **FIT** shows the whole route, **ZOOM** about 20 m around you |
| **Messenger** | The conversation, in Public or Secure mode |
| **Sensors** | Time, date, position, altitude, satellites, temperature, humidity |
| **Team radar** | Up to 5 teammates and how far away they are |
| **Books** | Novels sent from the app. **C** turns the page, **A** goes back, **B** returns to the list |
| **Settings** | Power mode, language and time zone |

**Shortcuts** (press together):

| Buttons | Action |
|---|---|
| **A + B** | Start or stop recording your track |
| **B + C** | SOS |
| **A + C** | Deep sleep. GLYPH wakes when it is moved or a button is pressed |

## Messages

- **Public**: every GLYPH in range can read it.
- **Secure**: encrypted with AES-256. Only units with the same **team name** can read it, and only
  they appear on each other's radar. Set the team name in **Control › Team channel**; it is the key,
  so share it in person.
- Messages hop through up to 3 other units on their way. Under each message you send, the app says
  whether it went out on air.
- Range is 10 km and more with a clear line of sight; less in forests, valleys and between buildings.
  Higher is better: a ridge or a window helps more than anything.

## Routes and maps

**Plan a route** in the **Routes** tab: tap the pencil, choose **Auto route**, type where you start
and where you are going, and pick on foot, by bike or by car. Or choose **Draw** and tap the map.

**Send it to GLYPH**:

- **Send to GLYPH** puts the route in GLYPH's own memory. It is shown on the map right away and stays
  there after sleep, a restart or an update.
- The small **card** button saves it to the memory card only, for later.

**Maps without signal**: the detailed map needs internet the first time you look at a place.
Before you go:

- **Save area** (the map button on the right) keeps what is on the screen.
- **Save for this route** (next to Send to GLYPH) keeps a 2 km band along your route.

Without signal you still see every saved place in detail, and a world map with towns, forests and
borders everywhere else. Saved areas can be deleted in **Control › App**.

## Recording a track

Press **A + B** on GLYPH, or use **Control › On the device** in the app. GLYPH records your track to the memory card as
**KML**, and the app draws it live while it is connected. Tracks are in the **Files** tab, where you
can export them. KML files open in Google Earth and most map apps.

## SOS

Press **B + C** on GLYPH, or **Control › On the device › Broadcast SOS** in the app. GLYPH broadcasts your position to every
unit in range, over and over, until it is cancelled. Messages from an SOS travel further through the
mesh (4 relays).

**To stop it**, press any button on GLYPH, or stop it from the app.

> ⚠️ GLYPH is not a certified emergency beacon. In a real emergency, also use every other way you
> have to call for help.

## Battery and power modes

| Mode | Bluetooth | GPS | LoRa | Battery* |
|---|---|---|---|---|
| **Normal** | On | Full power | On | ~32 h |
| **Eco** | Off | Power save | On | ~40 h |
| **Stealth** | Off | Power save | Off | ~6 days |
| **Deep sleep** (A + C) | Off | Off | Off | ~70 days |

<sub>* Estimates with a 3400 mAh 18650 cell.</sub>

Change the mode in **Control › Power mode** or on the device. In Eco and Stealth, Bluetooth is off:
switch back to Normal to use the app.

## Updates

- **The app** updates itself: a new version downloads in the background and starts the next time
  you open the app.
- **GLYPH** updates over Wi-Fi: **Control › Firmware › Update over Wi-Fi**. It checks every update
  before switching to it, and keeps the old firmware if something goes wrong.

## Troubleshooting

| Problem | Try this |
|---|---|
| The app does not find GLYPH | Bluetooth on, the *Nearby devices* permission allowed, and GLYPH in **Normal** mode. Tap the logo again |
| Pairing fails | **Control › Security › Forget paired phones** on the app side, remove GLYPH from Android's Bluetooth settings, and pair again |
| *GLYPH is not plugged in* when installing | Use a cable that carries data: many USB-C cables only charge |
| The access code is refused | Check it letter by letter. It came with your GLYPH; ask the seller if you lost it |
| Status shows **NO** and a module name | That module does not answer: check its Qwiic plug, then run the [diagnostic](../diagnostic/README.md) |
| GLYPH cannot update over Wi-Fi | The app tells you if its firmware is too old: install once over USB-C |
| No detailed map without signal | Only saved or already viewed places are offline. Save the area or the route before you go |

Still stuck? [Open an issue](https://github.com/Mqr1oo/GLYPH/issues).
