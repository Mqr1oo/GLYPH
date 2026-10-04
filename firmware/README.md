# 🔒 GLYPH firmware

The software inside GLYPH: the LoRa radio and mesh, encryption, GPS, the e-ink interface, the
memory card, Bluetooth and updates over Wi-Fi.

> **The source code is private.** The firmware comes with your GLYPH: the app installs it with the
> access code that comes with every GLYPH. See [LICENSE](../LICENSE).

**Latest version: 04.10.26** (4 October 2026) · [What's new](../CHANGELOG.md)

---

## Install

The firmware travels inside the [GLYPH app](../app/README.md) and goes onto GLYPH over a USB-C cable:
no computer and no internet needed.

1. Plug GLYPH into your Android phone with a USB-C cable that carries data (some only charge).
2. Open the app. The first screen installs the firmware; later it is under
   **Control › App › Install firmware over USB-C**.
3. Enter the **access code** that came with your GLYPH. The app asks for it only once.
4. Tap **Install** and allow USB access if Android asks. GLYPH restarts after about a minute.

GLYPH keeps its settings and Bluetooth pairing. Tick *Erase everything* for a factory reset.

## Updates over Wi-Fi

Once installed, GLYPH updates itself. In the app, open **Control › Firmware › Update over Wi-Fi**.
GLYPH downloads the new version, checks it against its published SHA-256, and only then switches
to it. If the update is interrupted, it keeps the firmware it already had.

> A GLYPH running a very old build (with a single application slot) cannot update itself, and the
> app tells you so. Install the current firmware once over USB, and it updates itself from then on.

## Version numbers

A version is the date it was built, as `DD.MM.YY`: **04.10.26** is 4 October 2026. A second build on
the same day gets `.2`, `.3` and so on. The app compares versions as dates.

## What it runs on

The [LilyGO T3-S3 E-Paper](../hardware/README.md) board (ESP32-S3, SX1262 LoRa radio, 2.13" e-ink
screen) with the GLYPH modules plugged in. Something not answering after assembly? Run the
[diagnostic](../diagnostic/README.md) first.
