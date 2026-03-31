<div align="center">

# 🛰️ GLYPH OS

### Beyond the Signal: Tactical Communication & Navigation

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=for-the-badge&logo=espressif" alt="ESP32-S3"/>
  <img src="https://img.shields.io/badge/Radio-LoRa_SX1262-green?style=for-the-badge" alt="LoRa"/>
  <img src="https://img.shields.io/badge/Security-AES--256-red?style=for-the-badge&logo=letsencrypt" alt="AES-256"/>
  <img src="https://img.shields.io/badge/Display-E--Ink-black?style=for-the-badge" alt="E-Ink"/>
</p>

<p align="center">
  <img src="https://img.shields.io/github/license/Mqr1oo/GLYPH?style=flat-square" alt="License"/>
  <img src="https://img.shields.io/github/stars/Mqr1oo/GLYPH?style=flat-square" alt="Stars"/>
  <img src="https://img.shields.io/github/forks/Mqr1oo/GLYPH?style=flat-square" alt="Forks"/>
  <img src="https://img.shields.io/github/issues/Mqr1oo/GLYPH?style=flat-square" alt="Issues"/>
</p>

**An ESP32-based hybrid OS built for tactical operations, extreme trekking, and survival scenarios.**  
Military-grade encrypted LoRa messaging • Offline GNSS navigation • Real-time team tracking • Ultra-low-power E-Ink display

[📥 Download](https://github.com/Mqr1oo/GLYPH/releases) • [📖 Documentation](#) • [🐛 Report Bug](https://github.com/Mqr1oo/GLYPH/issues) • [💡 Request Feature](https://github.com/Mqr1oo/GLYPH/issues)

---

</div>

## 📸 Screenshots

> **TODO**: Add device photos and UI screenshots here
> ```markdown
> ![Device Photo](docs/images/device.jpg)
> ![Map Interface](docs/images/map-ui.png)
> ![Messenger](docs/images/messenger.png)
> ```

---

## 🎯 Why GLYPH OS?

Traditional communication systems fail when you need them most. GLYPH OS was engineered from the ground up for **operations beyond the grid**:

- 🔒 **Zero-trust security** - Military-grade AES-256 encryption on every transmission
- 📡 **Off-grid comms** - LoRa mesh networking with 10km+ range (line-of-sight)
- 🗺️ **Offline navigation** - Full GPS tracking without cellular/satellite dependence
- 👥 **Squad awareness** - Real-time team position tracking on your wrist
- 🔋 **Extended missions** - Days of runtime on a single charge with intelligent power modes
- 🌞 **Extreme conditions** - Sunlight-readable E-Ink display, operates -20°C to 60°C

---

## 📑 Table of Contents

- [🔥 Core Capabilities](#-core-capabilities)
  - [🗺️ Offline Tactical Map & Navigation](#️-offline-tactical-map--navigation)
  - [📡 Encrypted LoRa Messenger](#-encrypted-lora-messenger)
  - [👥 Team Radar](#-team-radar)
- [🥷 Stealth & Survivability](#-stealth--survivability)
- [🧰 Hardware & Components](#-hardware--components)
- [💻 Installation & Setup](#-installation--setup)
  - [Prerequisites](#prerequisites)
  - [Firmware Upload](#firmware-upload)
- [🆘 Emergency & Connectivity](#-emergency--connectivity)
- [📚 Usage Guide](#-usage-guide)
- [🛣️ Roadmap](#️-roadmap)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)
- [⭐ Acknowledgments](#-acknowledgments)

---

## 🔥 Core Capabilities

### 🗺️ Offline Tactical Map & Navigation

<table>
<tr>
<td width="50%">

**Live Breadcrumbs**  
Real-time route drawing with advanced GPS noise filtering—only records true movement **> 20cm** for accurate trail tracking.

**KML Overlays**  
Load pre-planned tactical routes directly from SD card. Compatible with ATAK, Google Earth, and standard GIS tools.

**Live Telemetry**  
Pinpoint distance and altitude tracking with **3 dynamic zoom levels** for tactical situational awareness.

</td>
<td width="50%">

```
📍 Current Position
├─ Lat: 45.7489° N
├─ Lon: 21.2087° E
├─ Alt: 287m ASL
├─ Speed: 4.2 km/h
└─ Heading: 047° NE

📏 Trip Stats
├─ Distance: 12.4 km
├─ Duration: 3h 45m
└─ Avg Speed: 3.3 km/h
```

</td>
</tr>
</table>

### 📡 Encrypted LoRa Messenger

<table>
<tr>
<td width="50%">

**Military-Grade Security**  
- **AES-256-CBC** encryption standard
- Keys dynamically generated via **SHA-256** hashing of Team Name
- Prevents unauthorized interception and message spoofing

**Invisible Geolocation**  
Automatically attaches **hidden GPS coordinates** to every transmitted message for automatic squad position updates.

**Smart Input**  
- Physical 3-button interface for field operation
- Bluetooth keyboard pairing for rapid message composition

</td>
<td width="50%">

```
🔐 Message Security
├─ Algorithm: AES-256-CBC
├─ Key Derivation: SHA-256(TeamName)
├─ IV: Random per message
└─ Auth: HMAC-SHA256

📤 Transmission
├─ Frequency: 868/915 MHz
├─ Spreading Factor: 7-12 (adaptive)
├─ Bandwidth: 125 kHz
├─ TX Power: 2-22 dBm
└─ Range: 2-10+ km (terrain-dependent)
```

</td>
</tr>
</table>

### 👥 Team Radar

**Live Squad Tracking**  
Monitor up to **5 team members** simultaneously with automatic position updates extracted from incoming LoRa traffic.

**Auto-Extraction**  
Intelligently parses coordinates from received messages—no manual input required.

**Status Monitoring**  
Built-in **"Last Seen"** timestamps show data freshness and reliability for each squad member.

```
👤 ALPHA-1    ✅ Active (12s ago)    Distance: 247m    Bearing: 135° SE
👤 ALPHA-2    ✅ Active (8s ago)     Distance: 189m    Bearing: 042° NE
👤 ALPHA-3    ⚠️  Stale (2m ago)     Distance: 1.2km   Bearing: 310° NW
👤 BRAVO-1    ⚠️  Stale (5m ago)     Distance: 3.8km   Bearing: 270° W
👤 BRAVO-2    ❌ Lost (15m ago)      Distance: ---     Bearing: ---
```

---

## 🥷 Stealth & Survivability

GLYPH OS features a **dynamic power management engine** optimized for extended tactical operations:

| Mode | GPS Update | LoRa RX | TX Power | Battery Life* | Use Case |
|------|------------|---------|----------|---------------|----------|
| **🎯 TACTICAL** | 5s | Active | 22 dBm | ~18h | Active operations, team coordination |
| **🌲 ECO** | 10s | Active | 17 dBm | ~36h | Long-range trekking, patrol mode |
| **🥷 STEALTH** | 50s | **OFF** | **OFF** | ~7 days | Radio silence, evasion, surveillance detection avoidance |

**\*** *Battery life based on 3000mAh Li-Ion battery*

### Additional Survivability Features

- **Anti-Ghosting E-Ink**: Zero-glare, ultra-low power consumption (only draws power during screen refresh)
- **Sunlight Readable**: Full visibility in direct sunlight—no backlight required
- **Environmental Rating**: Operates reliably from -20°C to +60°C
- **Low EMI Profile**: Minimal electromagnetic signature in STEALTH mode

---

## 🧰 Hardware & Components

### Bill of Materials

| Component | Specification | Description | Link |
|-----------|--------------|-------------|------|
| **MCU** | ESP32-S3 Dual-Core | Handles heavy cryptography and UI processing | [ESP32-S3](https://www.espressif.com/en/products/socs/esp32-s3) |
| **Display** | 2.13" E-Ink (GxEPD2) | High-contrast, sunlight-readable screen | [Waveshare 2.13"](https://www.waveshare.com/product/displays/e-paper.htm) |
| **Radio** | SX1262 LoRa | 868MHz/915MHz with adjustable power (up to 22dBm) | [SX1262](https://www.semtech.com/products/wireless-rf/lora-core/sx1262) |
| **GNSS** | u-blox GNSS (I2C) | Fast time-to-first-fix and high accuracy | [u-blox NEO](https://www.u-blox.com/en/product/neo-m9n-module) |
| **IMU** | LSM6DSOX | 6-Axis accelerometer + gyroscope | [LSM6DSOX](https://www.st.com/en/mems-and-sensors/lsm6dsox.html) |
| **Magnetometer** | LIS3MDL | Tilt-compensated compass | [LIS3MDL](https://www.st.com/en/mems-and-sensors/lis3mdl.html) |
| **Storage** | MicroSD Card | KML route storage and telemetry logging | Any Class 10+ SD |
| **Battery** | 3000mAh Li-Ion | 18650 or similar form factor | - |

### Schematic & PCB

> **Coming Soon**: Full schematics and Gerber files for PCB fabrication
> 
> - [ ] Schematic diagram (KiCad)
> - [ ] PCB layout files
> - [ ] 3D-printable enclosure STL

---

## 💻 Installation & Setup

### Prerequisites

Before you begin, ensure you have the following installed:

- **PlatformIO IDE** (recommended) or **Arduino IDE 2.0+**
- **Python 3.7+** (for PlatformIO)
- **Git**

### Required Libraries

The following libraries are automatically installed via PlatformIO:

```ini
[env:esp32-s3]
platform = espressif32
framework = arduino
lib_deps = 
    jgromes/RadioLib@^6.0.0
    ZinggJM/GxEPD2@^1.5.0
    sparkfun/SparkFun u-blox GNSS Arduino Library@^2.2.0
    adafruit/Adafruit LSM6DS@^4.5.0
    adafruit/Adafruit LIS3MDL@^2.1.0
```

### Firmware Upload

#### Using PlatformIO (Recommended)

```bash
# 1. Clone the repository
git clone https://github.com/Mqr1oo/GLYPH.git
cd GLYPH

# 2. Open in PlatformIO or build from CLI
pio run -t upload

# 3. Monitor serial output (optional)
pio device monitor
```

#### Using Arduino IDE

1. Install required libraries via Library Manager:
   - RadioLib
   - GxEPD2
   - SparkFun u-blox GNSS
   - Adafruit LSM6DS & LIS3MDL

2. Open `GLYPH.ino`

3. Select **ESP32-S3 Dev Module** from Tools > Board

4. Configure settings:
   - Upload Speed: `921600`
   - Flash Size: `8MB (64Mb)`
   - Partition Scheme: `Default 4MB with spiffs`

5. Click **Upload**

### Initial Configuration

1. **Set Team Name** (for encryption key generation)
2. **Configure LoRa frequency** (868MHz EU / 915MHz US)
3. **Load KML routes** to SD card (optional)
4. **Pair Bluetooth** for keyboard input (optional)

---

## 🆘 Emergency & Connectivity

### Instant SOS Broadcast

**Dedicated hardware button combo** (B + C held for 3 seconds):
- Broadcasts continuous SOS signal at **maximum LoRa range**
- Uses **Spread Factor 12** for maximum distance (up to 20km+ line-of-sight)
- Transmits GPS coordinates every 10 seconds
- Overrides all power modes—device stays in emergency TX mode

### BLE Tactical Modem

**WebBluetooth Gateway** for smartphone connectivity:
- Connect via mobile browser (no app required)
- Receive telemetry and message notifications
- Send messages using smartphone keyboard
- **No cellular network required** — pure Bluetooth LE

```
📱 Supported Browsers
├─ Chrome/Edge (Android, Windows, macOS)
├─ Bluefy (iOS - WebBluetooth wrapper)
└─ Safari (iOS 16.4+ experimental)
```

### Black Box Logging

**Automatic mission data recording:**

- **Movement Logs**: `.KML` route files (Google Earth / ATAK compatible)
- **Telemetry Data**: `.CSV` files with full sensor data
- **Message Archive**: Encrypted message log with timestamps
- **Event Markers**: Waypoints, SOS activations, team contacts

```
📂 SD Card Structure
/GLYPH
├─ /routes
│  ├─ 2024-01-15_mission_alpha.kml
│  └─ 2024-01-16_patrol.kml
├─ /telemetry
│  ├─ 2024-01-15_log.csv
│  └─ 2024-01-16_log.csv
└─ /messages
   └─ archive_2024-01.txt
```

---

## 📚 Usage Guide

### Basic Operation

1. **Power On**: Hold power button for 2 seconds
2. **Navigation**: Use 3-button interface
   - **A**: Up/Previous
   - **B**: Select/Confirm
   - **C**: Down/Next
3. **Send Message**: 
   - Navigate to Messenger
   - Type using buttons or pair Bluetooth keyboard
   - Press B to send

### Power Mode Switching

- **Single press Power button**: Cycle through modes (TACTICAL → ECO → STEALTH)
- **Current mode** displayed on status bar

### Loading Custom Routes

1. Prepare `.KML` file in Google Earth or ATAK
2. Copy to SD card: `/GLYPH/routes/`
3. Device auto-loads routes on boot
4. Select route from Map menu

---

## 🛣️ Roadmap

- [ ] **v1.1** - Voice message encryption & transmission
- [ ] **v1.2** - Mesh networking (multi-hop relay)
- [ ] **v1.3** - Integration with ATAK-CIV (Cursor-on-Target)
- [ ] **v2.0** - Satellite messenger integration (Iridium)
- [ ] **v2.1** - Image transmission (low-bandwidth tactical photos)
- [ ] Hardware refresh with longer battery life

**See the [open issues](https://github.com/Mqr1oo/GLYPH/issues) for a full list of proposed features and known issues.**

---

## 🤝 Contributing

Contributions are what make the open-source community amazing! Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- Follow existing code style
- Comment complex cryptography/radio logic
- Test on real hardware before PR
- Update documentation for new features

---

## 📄 License

Distributed under the **MIT License**. See `LICENSE` file for more information.

---

## ⭐ Acknowledgments

- [RadioLib](https://github.com/jgromes/RadioLib) - Excellent LoRa abstraction layer
- [GxEPD2](https://github.com/ZinggJM/GxEPD2) - E-Ink display driver
- [u-blox GNSS](https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library) - GPS/GNSS library
- Community feedback from tactical/survival forums

---

<div align="center">

**Built with ❤️ for operators, adventurers, and survivalists**

[⬆ Back to Top](#-glyph-os)

</div>
