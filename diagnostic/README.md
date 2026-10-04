# ✅ GLYPH Diagnostic

A small **open-source** firmware that checks a freshly assembled GLYPH: it shows on the e-ink screen
which modules answer, so you can find a loose cable before installing the real firmware.

| Checked | How |
|---|---|
| **microSD card** | The card is mounted |
| **SHT40** (temperature and humidity) | Answers on I²C address `0x44` |
| **LSM6DSOX** (motion sensor) | Answers on I²C address `0x6A` |
| **SAM-M8Q** (GPS) | Answers on I²C address `0x42` |
| **Modulino Buttons** | Found on the Qwiic bus |

The buttons switch pages: **A** shows the module list. A module marked as missing is almost always
a Qwiic plug that is not fully in.

## Install it

**From the browser:** open [mqr1oo.github.io/GLYPH](https://mqr1oo.github.io/GLYPH/) in Chrome or
Edge on a computer, plug GLYPH in with a USB-C cable and follow the steps.

**From Arduino IDE:** open [`GLYPH_Diagnostic/GLYPH_Diagnostic.ino`](GLYPH_Diagnostic/GLYPH_Diagnostic.ino).

| Setting | Value |
|---|---|
| Board | ESP32S3 Dev Module (esp32 core 3.x) |
| USB CDC on boot | Enabled |
| Libraries | GxEPD, U8g2_for_Adafruit_GFX, Arduino_Modulino |

When everything answers, install the GLYPH [firmware](../firmware/README.md) from the app.

## License

GPL-3.0, see [LICENSE](LICENSE). This is the only open-source part of GLYPH; the GLYPH firmware and
app are closed source.
