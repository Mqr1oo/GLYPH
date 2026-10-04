# 🛠️ GLYPH hardware

GLYPH is built from standard modules that plug together with **Qwiic** cables, in a 3D-printed case.
No soldering, no tools, about 30 minutes.

<p align="center">
  <img src="../docs/images/qwiic.jpg" width="80%" alt="The parts of GLYPH laid out: case, lid, buttons cover, battery, radio board, GPS, IMU, sensor and buttons"/>
</p>

## Parts

| Part | What it does |
|---|---|
| **LilyGO T3-S3 E-Paper** (868 or 915 MHz) | ESP32-S3 brain, LoRa SX1262 radio and 2.13" e-ink screen |
| **SparkFun GNSS SAM-M8Q** (Qwiic) | GPS |
| **Adafruit LSM6DSOX + LIS3MDL** (Qwiic) | 9-axis motion sensor and compass |
| **Adafruit SHT40** (Qwiic) | Temperature and humidity |
| **Arduino Modulino Buttons** (Qwiic) | The three buttons, A, B and C |
| **5 × Qwiic cables** (5 cm) | Connect everything |
| **18650 Li-ion cell** (3400 mAh) and holder | Power |
| **ARK connector** (3.5 mm, 2-pin) or screw terminal | Connects the battery holder |
| **microSD card** (8–16 GB) | Tracks, messages and logs |

The parts cost about **160–190 €**.

## Radio band

Buy the board for the band allowed where you use it:

| Band | Where |
|---|---|
| **868 MHz** | Europe, the UK, India and most of Africa |
| **915 MHz** | The Americas, Australia and New Zealand |

The band is also set in the app (**Control › Device › Radio band**). Every GLYPH in a team must use
the same band.

## The case

- Body in **PETG-CF**, button caps in **TPU**, printed on a Bambu Lab printer with an AMS.
- The print files and the full kit will be published on **MakerWorld**.

## Assembly checks

- ⚠️ **Screw the antenna on before the first power-up.** A LoRa radio that transmits without an
  antenna can damage itself.
- Mind the battery's polarity. Use a good-quality 18650 cell.
- Push every Qwiic plug in fully: a loose plug is the most common reason a module does not answer.
- Insert the microSD card before closing the case.

## After assembly

1. Run the **[diagnostic](../diagnostic/README.md)**: it shows which modules answer.
2. Install the **[firmware](../firmware/README.md)** from the app, with the access code that comes
   with your GLYPH.
3. Read the **[user guide](../docs/guide.md)**.
