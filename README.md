<div align="center">

# 🛰️ GLYPH OS

### Beyond the Signal: Tactical Communication & Navigation

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-blue?style=for-the-badge&logo=espressif" alt="ESP32-S3"/>
  <img src="https://img.shields.io/badge/Radio-LoRa_SX1262-green?style=for-the-badge" alt="LoRa"/>
  <img src="https://img.shields.io/badge/Security-AES--256--CBC-red?style=for-the-badge&logo=letsencrypt" alt="AES-256"/>
  <img src="https://img.shields.io/badge/Display-E--Ink_2.13"-black?style=for-the-badge" alt="E-Ink"/>
  <img src="https://img.shields.io/badge/Cores-Dual--Core-orange?style=for-the-badge" alt="Dual-Core"/>
</p>

<p align="center">
  <img src="https://img.shields.io/github/license/Mqr1oo/GLYPH?style=flat-square" alt="License"/>
  <img src="https://img.shields.io/github/stars/Mqr1oo/GLYPH?style=flat-square" alt="Stars"/>
  <img src="https://img.shields.io/github/forks/Mqr1oo/GLYPH?style=flat-square" alt="Forks"/>
  <img src="https://img.shields.io/github/issues/Mqr1oo/GLYPH?style=flat-square" alt="Issues"/>
  <img src="https://img.shields.io/badge/Version-10.08.26-blue?style=flat-square" alt="Version"/>
</p>

**An ESP32-S3 dual-core tactical operating system for extreme operations.**  
Military-grade AES-256-CBC encryption • Offline GNSS navigation • Real-time team tracking • Ultra-low-power E-Ink display • Multi-language support (11 languages)

[📥 Download](https://github.com/Mqr1oo/GLYPH/releases) • [📖 Documentation](#-documentation) • [🐛 Report Bug](https://github.com/Mqr1oo/GLYPH/issues) • [💡 Request Feature](https://github.com/Mqr1oo/GLYPH/issues)

---

</div>

## 📸 Screenshots & Demo

> **Add your device photos here**
> ```markdown
> ![GLYPH Device](docs/images/device.jpg)
> ![Tactical Map Interface](docs/images/map-ui.png)
> ![Encrypted Messenger](docs/images/messenger.png)
> ![Team Radar](docs/images/team-radar.png)
> ```

---

## 🎯 Why GLYPH OS?

GLYPH OS is a **fully autonomous tactical communication and navigation system** that operates when all other systems fail:

- 🔒 **Military-Grade Encryption** - AES-256-CBC with SHA-256 key derivation from team names
- 📡 **Long-Range LoRa** - Up to 10km+ range (SF7-SF12 adaptive, 2-22dBm TX power)
- 🗺️ **Offline GPS Tracking** - Smart breadcrumb trail with <20cm noise filtering
- 👥 **Team Awareness** - Track 5 teammates with automatic coordinate extraction
- 🔋 **Extended Runtime** - 18h (TACTICAL) → 36h (ECO) → 7 days (STEALTH) on 3000mAh
- 🌞 **Extreme Conditions** - Sunlight-readable E-Ink, operates -20°C to 60°C
- 🧠 **Dual-Core Architecture** - Core 0: Sensor polling | Core 1: UI & Logic
- 🌍 **Multi-Language** - 11 languages: RO, EN, ES, FR, DE, IT, PT, PL, NL, TR, CN

---

## 📑 Table of Contents

- [🔥 Core Capabilities](#-core-capabilities)
  - [🗺️ Tactical Map & Navigation](#️-tactical-map--navigation)
  - [📡 Encrypted LoRa Messenger](#-encrypted-lora-messenger)
  - [👥 Team Radar](#-team-radar)
  - [🧭 Tilt-Compensated Compass](#-tilt-compensated-compass)
- [🥷 Power Management](#-power-management)
- [🧰 Hardware Requirements](#-hardware-requirements)
- [💻 Installation Guide](#-installation-guide)
- [⚙️ Configuration](#️-configuration)
- [🆘 Emergency Features](#-emergency-features)
- [📊 Technical Architecture](#-technical-architecture)
- [🛣️ Roadmap](#️-roadmap)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## 🔥 Core Capabilities

### 🗺️ Tactical Map & Navigation

**Smart GPS Breadcrumb System**
- **Noise Filtering**: Only records true movement >20cm (prevents GPS drift)
- **Breadcrumb Buffer**: Stores up to 350 waypoints in RTC memory (survives deep sleep)
- **Distance Calculation**: Haversine formula for accurate ground distance
- **Automatic Logging**: Real-time KML generation with timestamp-based filenames

**KML Route Overlay System**
```cpp
// Cached KML parsing for fast rendering
loadKMLCache()  → Parses bounds & distance
drawKMLOverlay() → Renders directly from SD without RAM loading
```
- Load pre-planned routes from SD card
- Automatic bounding box calculation
- Distance pre-calculation for display
- Compatible with Google Earth, ATAK, and standard GIS tools

**Triple Zoom System**
| Zoom Level | Description | Use Case |
|------------|-------------|----------|
| **FIT** | Auto-zoom to show entire route + KML | Route overview, mission planning |
| **ZOOM** | Fixed ~20m radius around position | Close navigation, tactical ops |
| **TACTICAL** | Dynamic zoom based on activity | General navigation |

**Live Recording**
```cpp
Recording Start:
├─ Creates: /route_DD_MM_YYYY_HH-MM.kml
├─ Appends coordinates in real-time
└─ On Stop: Renames to /route_DD_MM_YYYY_HH-MM_to_HH-MM.kml

GPS Logging Trigger:
└─ Movement > 0.2m from last logged position
```

### 📡 Encrypted LoRa Messenger

**Military-Grade Encryption**
```cpp
// AES-256-CBC Implementation
Key Generation:  SHA-256(Team Name) → 256-bit key
IV:              Random 16 bytes per message
Padding:         PKCS#7 standard
Message Format:  0xFF + IV(32 hex) + Ciphertext(hex)
```

**Transmission Protocol**
```cpp
Message Structure:
├─ Plaintext: "USERNAME: Message|LAT,LON"
├─ Encrypted: 0xFF + [IV + AES(plaintext)]
└─ Broadcast: LoRa @ SF11, 125kHz BW, CR 4/8

Security Features:
├─ Dynamic IV prevents replay attacks
├─ Team-based encryption (different teams = different keys)
└─ Hidden GPS coordinates in every message
```

**Spread Factor System**
| Mode | SF | Range | Data Rate | Use Case |
|------|----|----|-----------|----------|
| Normal | SF11 | 2-5km | ~440 bps | Team comms |
| SOS | SF12 | 10-20km+ | ~183 bps | Emergency broadcast |
| Fast | SF7 | <500m | ~5.5 kbps | Close-range tactical |

**Auto-Coordinate Extraction**
```cpp
// Automatic teammate tracking
Received: "ALPHA-2: Moving north|45.7489,21.2087"
         ↓
Extracts: Name: "ALPHA-2", Lat: 45.7489, Lon: 21.2087
         ↓
Updates:  teammates[] array with last seen timestamp
```

### 👥 Team Radar

**Live Squad Tracking**
- Monitor up to **5 teammates** simultaneously
- Automatic position updates from LoRa traffic
- Distance & bearing calculation from your position
- "Last Seen" aging system (NOW / 2m / 15m ago)

**Position Management**
```cpp
teammate[] Structure:
├─ name:     String (callsign)
├─ lat/lon:  double (last known position)
└─ lastSeen: millis() timestamp

Aging System:
├─ < 1 min:  "NOW" (green indicator)
├─ 1-5 min:  "Xm" (yellow indicator)
└─ > 15 min: "LOST" (red indicator)
```

**Smart Array Management**
```cpp
if (teammateCount >= MAX_TEAMMATES) {
    // Evict oldest-seen teammate
    int oldest = findOldest(teammates[]);
    teammates[oldest] = newTeammate;
}
```

### 🧭 Tilt-Compensated Compass

**Advanced Sensor Fusion**
```cpp
Magnetometer (LIS3MDL):
├─ 155 Hz sampling rate
├─ Hard/soft iron calibration
└─ 60-second auto-calibration routine

IMU (LSM6DSOX):
├─ 6-axis motion tracking
├─ Tilt compensation via accelerometer
└─ Gyro-assisted heading smoothing

Fusion Algorithm:
├─ Calculate magnetic north from mag + accel cross product
├─ Predict heading using gyro integration
├─ Fuse: predicted_heading + 0.015 * (absolute - predicted)
└─ Apply: Magnetic declination (5.5°) + Mount offset (270°)
```

**Calibration Process**
```cpp
Duration: 60 seconds
Method:   Full 360° rotation while level
Stores:   Min/max X,Y,Z → Calculates offset & scale
Saved to: NVS (survives reboot)
```

---

## 🥷 Power Management

**Intelligent Multi-Mode System**

| Mode | CPU | GPS Rate | LoRa | Battery* | Details |
|------|-----|----------|------|----------|---------|
| **🎯 TACTICAL** | 80 MHz | 1s | Active (SF11) | ~18h | Full operational capability |
| **🌲 ECO** | 40 MHz | 10s | Active (SF11) | ~36h | Extended range patrols |
| **🥷 STEALTH** | 40 MHz | 50s | **OFF** | ~7 days | Radio silence, zero emissions |

**\*** *Based on 3000mAh 18650 Li-Ion battery*

**Deep Sleep Architecture**
```cpp
Auto-Sleep Triggers:
├─ 4 min inactivity → STANDBY mode
│  ├─ Display off
│  ├─ GPS @ 60s intervals
│  ├─ BLE disabled
│  └─ CPU @ 20 MHz
│
└─ Manual shutdown (A+B+C) → DEEP SLEEP
   ├─ 8 min sleep timer
   ├─ GPS powered off
   └─ Wakeup: Button press OR timer
```

**Activity Detection System**
```cpp
Wake Sources:
├─ Button press (latched interrupts)
├─ IMU motion detection (>1.2g deviation)
└─ Timer wakeup (ECO mode GPS logging)
```

**Display Power Optimization**
```cpp
E-Ink Refresh Strategy:
├─ Full refresh: Every 15 min OR mode change
├─ Partial refresh: Data updates (heading, GPS, messages)
└─ No refresh: When unchanged (0 power consumption)
```

---

## 🧰 Hardware Requirements

### Core Components

| Component | Model | Interface | Description |
|-----------|-------|-----------|-------------|
| **MCU** | ESP32-S3 Dual-Core | - | 240MHz (2 cores), 512KB SRAM, WiFi/BLE |
| **Display** | 2.13" E-Ink (GxDEPG0213BN) | SPI | 250x122px, anti-ghosting, sunlight readable |
| **Radio** | SX1262 LoRa | SPI | 150-960 MHz, 22dBm max, -148 dBm sensitivity |
| **GNSS** | u-blox NEO-M9N | I2C | Multi-GNSS (GPS/GLONASS/Galileo/BeiDou) |
| **IMU** | LSM6DSOX | I2C | 6-axis (accel + gyro), ±16g, ±2000 dps |
| **Magnetometer** | LIS3MDL | I2C | 3-axis, ±16 gauss, tilt-compensated |
| **Temp/Humidity** | SHT40 | I2C | High precision, -40 to 125°C |
| **Buttons** | Modulino Buttons | I2C | 3-button tactile interface |
| **Storage** | MicroSD Card | SPI (HSPI) | Class 10+ recommended |
| **Battery** | 3000mAh Li-Ion | ADC (voltage monitoring) | 18650 or similar |

### Pin Configuration

**SPI Bus Assignments**
```cpp
// E-Ink Display (VSPI)
#define EDP_CS      15
#define EDP_DC      16
#define EDP_RESET   47
#define EDP_BUSY    48
// Uses default VSPI: SCLK=18, MISO=19, MOSI=23

// SD Card (HSPI)
#define SDCARD_CS   13
#define SDCARD_SCLK 14
#define SDCARD_MISO 2
#define SDCARD_MOSI 11

// LoRa Radio (VSPI)
#define RADIO_CS    7
#define RADIO_SCLK  5
#define RADIO_MISO  3
#define RADIO_MOSI  6
#define RADIO_DIO1  33
#define RADIO_RST   8
#define RADIO_BUSY  34
```

**I2C Bus**
```cpp
#define I2C_SDA     43
#define I2C_SCL     44
Speed: 400 kHz (Fast Mode)

Connected Devices:
├─ u-blox GNSS (0x42)
├─ SHT40 Temp/Humidity (0x44)
├─ LSM6DSOX IMU (0x6A)
├─ LIS3MDL Magnetometer (0x1C)
└─ Modulino Buttons (0x76)
```

**Power & Control**
```cpp
#define PIN_BAT_ADC     1  // Battery voltage monitor (7.26V max)
#define BTN_PIN         0  // Hardware button (ESP32 BOOT)
#define RADIO_POW_PIN   35 // LoRa module power control
```

### Schematic & PCB

> **TODO**: Upload full schematics and Gerber files
> - [ ] KiCad schematic (circuit diagram)
> - [ ] PCB layout files (Gerber + drill)
> - [ ] 3D enclosure STL (waterproof case)
> - [ ] Assembly guide

---

## 💻 Installation Guide

### Prerequisites

**Software Requirements:**
- **Arduino IDE 2.0+** (recommended) or **PlatformIO** (optional)
- **ESP32 Board Package** v2.0.9+
- **USB-C cable** for programming

### Required Libraries

**Install via Arduino Library Manager:**

```cpp
Core Libraries:
├─ RadioLib v6.0.0+          // LoRa SX1262 driver
├─ GxEPD v3.1.3+              // E-Ink display driver
├─ U8g2_for_Adafruit_GFX      // Font rendering
├─ SparkFun u-blox GNSS v2.2+ // GPS module
├─ Adafruit SHT4x             // Temp/humidity sensor
├─ Adafruit LSM6DS            // IMU (accel/gyro)
├─ Adafruit LIS3MDL           // Magnetometer
├─ Modulino                   // Button interface
└─ Button2                    // Button handling

System Libraries (ESP32 Core):
├─ SPI.h
├─ Wire.h
├─ SD.h
├─ Preferences.h
├─ WiFi.h
├─ BLEDevice.h
└─ mbedtls (AES encryption)
```

### Installation Steps

#### Option 1: Arduino IDE (Recommended)

```bash
# 1. Install ESP32 Board Support
Arduino IDE → Preferences → Additional Board Manager URLs:
https://espressif.github.io/arduino-esp32/package_esp32_index.json

# 2. Install ESP32 Boards
Tools → Board → Boards Manager → "ESP32" → Install

# 3. Clone Repository
git clone https://github.com/Mqr1oo/GLYPH.git
cd GLYPH

# 4. Open GLYPH.ino in Arduino IDE

# 5. Install Required Libraries
Sketch → Include Library → Manage Libraries
Search and install each library from the list above

# 6. Board Configuration
Tools → Board → ESP32 Arduino → ESP32S3 Dev Module
Tools → Upload Speed → 921600
Tools → Flash Size → 8MB (64Mb)
Tools → Partition Scheme → Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
Tools → PSRAM → Disabled (or QSPI PSRAM if your board has it)

# 7. Upload
Click Upload button or Ctrl+U
```

#### Option 2: PlatformIO (Advanced)

```bash
# 1. Clone repository
git clone https://github.com/Mqr1oo/GLYPH.git
cd GLYPH

# 2. Create platformio.ini (if not present)
cat > platformio.ini << EOF
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

lib_deps = 
    jgromes/RadioLib@^6.0.0
    ZinggJM/GxEPD@^3.1.3
    olikraus/U8g2_for_Adafruit_GFX@^1.8.0
    sparkfun/SparkFun u-blox GNSS Arduino Library@^2.2.0
    adafruit/Adafruit SHT4x Library@^1.0.0
    adafruit/Adafruit LSM6DS@^4.5.0
    adafruit/Adafruit LIS3MDL@^2.1.0
    adafruit/Adafruit BusIO@^1.14.0
    arduino-libraries/Modulino@^1.0.0
    lennarthennigs/Button2@^2.3.0

upload_speed = 921600
monitor_speed = 115200
EOF

# 3. Build and upload
pio run -t upload

# 4. Monitor serial output (optional)
pio device monitor
```

### Post-Upload Configuration

**First Boot Setup Wizard:**

1. **Language Selection** (A: Previous, C: Next, B: Confirm)
2. **LoRa Frequency** (868 MHz EU / 915 MHz US)
3. **Username Entry** (via phone or serial)
4. **UTC Timezone** (A: -1h, C: +1h, B: Save)
5. **Compass Calibration** (60s full rotation)

**Alternative: Serial Bypass**
```bash
# Connect via Arduino Serial Monitor (115200 baud)
# Send command:
bypass

# Instantly configures:
├─ Language: English
├─ Frequency: 868 MHz
├─ Username: "username"
├─ Timezone: UTC+2
└─ Skips compass calibration
```

---

## ⚙️ Configuration

### BLE Phone Interface

**WebBluetooth Connection**
```javascript
// Supported browsers
Chrome/Edge (Android, Windows, macOS)
Bluefy (iOS wrapper for WebBluetooth)
Safari (iOS 16.4+ experimental)

// Connection process
1. Power on GLYPH
2. Open browser → Bluetooth settings
3. Connect to "GLYPH"
4. Use phone keyboard for input
```

**Virtual Commands (BLE/Serial)**
```cpp
// Navigation shortcuts
"1@" → Jump to Map
"2@" → Jump to Messenger
"3@" → Jump to Clock/Sensors
"4@" → Jump to Team Radar
"5@" → Jump to Power Settings
"6@" → Jump to Language Settings

// Virtual button presses
"A@"  → Press A
"B@"  → Press B
"C@"  → Press C
"AL@" → Long-press A
"BL@" → Long-press B
"CL@" → Long-press C
"AB@" → Combo A+B (Start/Stop Recording)
"AC@" → Combo A+C (SOS Broadcast)

// System commands
"CMD_PWR:0" → Set TACTICAL mode
"CMD_PWR:1" → Set ECO mode
"CMD_PWR:2" → Set STEALTH mode
"CMD_SEC:ON"  → Enable secure mode
"CMD_SEC:OFF" → Disable secure mode
"CMD_TEAM:ALPHA" → Change team name

// Testing
"45.7489,21.2087" → Simulate GPS coordinates
"SIMOFF" → Disable GPS simulation
```

### SD Card Structure

```
📂 SD Card Root
├─ 📄 route_DD_MM_YYYY_HH-MM.kml        # Active recording
├─ 📄 route_DD_MM_YYYY_HH-MM_to_HH-MM.kml  # Completed routes
├─ 📄 sys_DD_MM_YYYY.csv                # Telemetry logs
├─ 📄 sec_DD_MM_YYYY.txt                # Encrypted message archive
└─ 📄 pub_DD_MM_YYYY.txt                # Public message archive

KML Format (auto-generated):
<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2">
  <Document>
    <name>GLYPH DD_MM_YYYY_HH-MM</name>
    <Placemark>
      <LineString>
        <coordinates>
          21.2087,45.7489,287.1
          21.2088,45.7490,287.3
          ...
        </coordinates>
      </LineString>
    </Placemark>
  </Document>
</kml>

CSV Format (telemetry):
Date,Time,Lat,Lon,Temp,Hum,AccX,AccY,AccZ,Event
15_04_2024,14:32:45,45.7489,21.2087,22.3,65.1,0.12,-0.05,9.78,Auto_Log
```

### Power Mode Tuning

```cpp
// Modify in GLYPH.ino for custom profiles
if (currentPowerMode == NORMAL_MODE) {
    setCpuFrequencyMhz(80);              // CPU speed
    myGNSS.setMeasurementRate(1000);     // GPS rate (ms)
    radio.setOutputPower(22);             // TX power (dBm)
    radio.setSpreadingFactor(11);         // Range vs speed
}
```

---

## 🆘 Emergency Features

### SOS Broadcast (Hold A+C)

**Automatic Emergency Protocol**
```cpp
Trigger: Hold A+C for 3 seconds
Action:
├─ Switch to SF12 (maximum range ~20km)
├─ Broadcast every 100ms for 10 seconds
├─ Message: "USERNAME SOS! LAT:XX.XXXXX LON:XX.XXXXX"
└─ Append to message history

Duration: 10 seconds continuous
TX Count: ~100 transmissions
Coverage: Line-of-sight up to 20km+ (terrain dependent)
```

### Recording Toggle (Hold A+B)

```cpp
Trigger: Hold A+B for 1 second
Action:
├─ START: Create /route_DD_MM_YYYY_HH-MM.kml
├─ STOP:  Rename to /route_DD_MM_YYYY_HH-MM_to_HH-MM.kml
└─ Reset breadcrumb buffer & distance counter
```

### Manual Shutdown (A+B+C)

```cpp
Trigger: Press all 3 buttons simultaneously
Action:
├─ Display shutdown screen
├─ Enter deep sleep for 8 minutes
├─ GPS powered off
└─ Wake on: Button press OR timer
```

### Black Box Logging

**Automatic Data Recording**
```cpp
Telemetry Log (every 60s when recording):
├─ File: /sys_DD_MM_YYYY.csv
└─ Data: Date, Time, Lat, Lon, Temp, Hum, AccX/Y/Z, Event

Message Archive (all RX/TX):
├─ Secure: /sec_DD_MM_YYYY.txt
├─ Public:  /pub_DD_MM_YYYY.txt
└─ Format:  [HH:MM] USERNAME: Message

Route Files (real-time):
└─ File: /route_DD_MM_YYYY_HH-MM.kml
    ├─ Coordinates appended as you move
    └─ Compatible with ATAK, Google Earth, QGIS
```

---

## 📊 Technical Architecture

### Dual-Core Task Distribution

```cpp
// Core 1 (Protocol Core) - 240 MHz
├─ Main loop() - UI & Logic
│  ├─ Button event processing
│  ├─ LoRa RX/TX management
│  ├─ State machine execution
│  ├─ Display rendering
│  └─ Power management decisions
│
└─ Executes: setup(), loop(), updateUI()

// Core 0 (Application Core) - 240 MHz
├─ hardwareCore0Task() - Sensor Polling @ 30Hz
│  ├─ I2C mutex-protected sensor reads
│  ├─ Button debouncing (33ms cycle)
│  ├─ IMU motion detection
│  ├─ Magnetometer fusion
│  ├─ GPS PVT acquisition (1Hz)
│  └─ Temp/Humidity updates (1Hz)
│
└─ Priority: 2 (higher than idle)
```

### Memory Management

```cpp
// RTC Memory (survives deep sleep)
RTC_DATA_ATTR SystemState currentState;
RTC_DATA_ATTR GeoPoint breadcrumbs[350];  // ~2.8 KB
RTC_DATA_ATTR double sessionTotalDistance;
RTC_DATA_ATTR float currentHeading;
// ... other persistent variables

// NVS (Non-Volatile Storage)
Preferences prefs;
├─ Language setting
├─ LoRa frequency
├─ Username & team name
├─ UTC offset
├─ Compass calibration data
└─ Version tracking

// Heap Usage
├─ Display buffer: ~8 KB (E-Ink framebuffer)
├─ LoRa message history: ~1 KB (5 messages x 200 bytes)
├─ KML cache: ~1 KB (bounding box + distance)
└─ Teammate array: ~400 bytes (5 x 80 bytes)
```

### State Machine

```cpp
enum SystemState {
    // Setup Flow
    PAGE_INIT_LANG,     // Language selection
    PAGE_INIT_FREQ,     // LoRa frequency (868/915 MHz)
    PAGE_INIT_NAME,     // Username entry
    PAGE_INIT_TIME,     // UTC timezone
    PAGE_INIT_COMPASS,  // Magnetometer calibration
    
    // Main Screens
    PAGE_MAP,           // Tactical map with breadcrumbs
    PAGE_KML_LIST,      // Route file browser
    PAGE_CLOCK,         // Time & sensors
    PAGE_LORA,          // Message inbox
    PAGE_KEYBOARD,      // Message composer
    
    // Settings
    PAGE_MENU_MAIN,     // Main menu hub
    PAGE_MENU_TEAM,     // Team radar
    PAGE_TEAM_NAME_EDIT,// Team name editor
    PAGE_MENU_POWER,    // Power mode selector
    PAGE_MENU_LANG,     // Language settings
    PAGE_MENU_TIME,     // Timezone settings
    
    // Power States
    STANDBY_MODE        // Low-power idle
};
```

### Security Implementation

**AES-256-CBC Encryption**
```cpp
// Key Derivation
String teamName = "ALPHA";
SHA-256(teamName) → 256-bit key
    ↓
mbedtls_md_context_t → SHA256 hash
    ↓
32-byte AES key stored in RAM (never written to flash)

// Message Encryption Flow
Plaintext: "USER: Message|45.7489,21.2087"
    ↓
Generate Random IV (16 bytes)
    ↓
PKCS#7 Padding (to 16-byte blocks)
    ↓
AES-256-CBC Encryption
    ↓
Output: 0xFF + IV(32 hex chars) + Ciphertext(hex)
    ↓
LoRa Broadcast

// Security Features
├─ Unique IV per message (prevents replay attacks)
├─ Team-based keys (cross-team messages are gibberish)
├─ No key storage on SD (exists only in RAM)
└─ Automatic secure/public mode toggle
```

### Compass Fusion Algorithm

```cpp
// Tilt-Compensated Heading Calculation
1. Read Raw Magnetometer (X, Y, Z)
   ├─ Apply hard/soft iron calibration
   └─ Normalize field strength

2. Read Accelerometer (X, Y, Z)
   ├─ Low-pass filter (α = 0.95)
   └─ Calculate tilt angles

3. Calculate East Vector
   E = (mag × accel) / |mag × accel|

4. Calculate North Vector
   N = (accel × E) / |accel × E|

5. Compute Raw Heading
   heading = atan2(E_x, N_x) * 180/π

6. Gyro-Assisted Smoothing
   predicted = current_heading + (gyro_rate * dt)
   fused = predicted + 0.015 * (absolute - predicted)

7. Apply Corrections
   heading += MAGNETIC_DECLINATION (5.5°)
   heading += COMPASS_MOUNT_OFFSET (270°)
   heading = heading % 360°
```

---

## 📚 Usage Guide

### Basic Navigation

**3-Button Interface:**
```
A (Top)     → Up / Back / Previous
B (Middle)  → Select / Confirm / Add
C (Bottom)  → Down / Next / Forward
```

**Special Combos:**
```
A + B (hold 1s)  → Start/Stop route recording
A + C (hold 3s)  → SOS broadcast
B + C (hold 3s)  → (Reserved - unused)
A + B + C        → Manual shutdown
```

**Long Presses:**
```
Hold A (3s) → Exit keyboard mode
Hold B (3s) → Continuous delete in keyboard
Hold C (3s) → Send message / Save team name
```

### Operational Workflows

**Starting a Mission**
```
1. Power on → Boot to main menu
2. Navigate to "TACTICAL MAP"
3. Wait for GPS fix (SAT indicator)
4. Hold A+B → Start recording
5. Device logs breadcrumbs automatically
6. SD card saves KML in real-time
```

**Sending Encrypted Messages**
```
1. Navigate to "MESSENGER"
2. Press B → Enter keyboard mode
3. Type via BLE phone OR physical buttons
   ├─ A/C: Move cursor left/right
   ├─ B: Add selected character
   └─ Hold B: Delete last character
4. Hold C → Send (broadcasts with GPS coords)
5. Message appears in teammate's inbox
```

**Loading Route Overlays**
```
1. Copy .kml files to SD card root
2. Navigate to "TACTICAL MAP"
3. Press C → Open KML browser
4. A/C: Scroll through routes
5. B: Select route
6. Map displays overlay + your position
```

**Switching Power Modes**
```
1. Main Menu → "SYSTEM MODE"
2. A/C: Scroll modes (TACTICAL/ECO/STEALTH)
3. B: Confirm selection
4. Device adjusts CPU, GPS, radio automatically
```

### Tips & Tricks

**Maximize Battery Life:**
- Use ECO mode for long-range hiking (36h runtime)
- Switch to STEALTH when not communicating (7 days runtime)
- Disable BLE when not using phone keyboard

**Improve GPS Accuracy:**
- Allow 2-3 minutes for initial fix
- Position antenna with clear sky view
- Avoid urban canyons and dense forests

**Optimize LoRa Range:**
- Use elevated positions (hilltops, rooftops)
- Avoid valleys and obstacles
- For emergencies, use A+C SOS (SF12 = max range)

**Efficient KML Management:**
- Name routes descriptively before missions
- Use Google Earth to verify overlays offline
- Export ATAK overlays as standard KML

---

## 🛣️ Roadmap

### v1.1 - Voice Messaging
- [ ] Audio codec integration (Opus)
- [ ] Voice encryption (AES-256 + audio compression)
- [ ] Push-to-talk LoRa transmission
- [ ] Playback via piezo buzzer or external speaker

### v1.2 - Mesh Networking
- [ ] Multi-hop message relay
- [ ] Automatic route discovery
- [ ] Network topology mapping
- [ ] Packet routing optimization

### v1.3 - ATAK Integration
- [ ] Cursor-on-Target (CoT) message format
- [ ] Direct import of ATAK overlays
- [ ] Team position sharing via TAK protocol
- [ ] Mission package export

### v2.0 - Satellite Connectivity
- [ ] Iridium Short Burst Data (SBD) support
- [ ] Emergency satellite beacon
- [ ] Hybrid LoRa/satellite routing
- [ ] Global coverage capability

### v2.1 - Image Transmission
- [ ] Low-bandwidth SSTV encoding
- [ ] Camera module integration
- [ ] Tactical photo annotation
- [ ] Encrypted image LoRa broadcast

### Hardware Refresh
- [ ] Longer battery life (5000mAh+)
- [ ] External GPS antenna connector
- [ ] USB-C charging
- [ ] Waterproof IP67 enclosure

---

## 🤝 Contributing

Contributions make the open-source community thrive! Here's how you can help:

### Development Guidelines

**Code Style:**
```cpp
// Use descriptive variable names
float currentHeading;  // ✓ Good
float h;               // ✗ Bad

// Comment complex algorithms
// Calculate tilt-compensated heading using mag + accel fusion
float heading = calculateHeading(mag, accel);

// Group related functionality
void processCompassData() {
    readMagnetometer();
    readAccelerometer();
    fuseHeadingData();
}
```

**Pull Request Process:**
1. Fork the repository
2. Create feature branch: `git checkout -b feature/AmazingFeature`
3. Test on real hardware (ESP32-S3 + peripherals)
4. Update README if adding features
5. Commit: `git commit -m 'Add AmazingFeature'`
6. Push: `git push origin feature/AmazingFeature`
7. Open Pull Request with detailed description

**Priority Areas:**
- Battery optimization techniques
- LoRa range improvements
- GPS accuracy enhancements
- UI/UX refinements
- Multi-language translations
- Documentation improvements

### Bug Reports

When filing issues, include:
- GLYPH OS version (shown on boot)
- Hardware configuration (board, sensors)
- Steps to reproduce
- Expected vs actual behavior
- Serial output logs

---

## 📄 License

Distributed under the **MIT License**. See `LICENSE` file for details.

```
MIT License

Copyright (c) 2024 Mqr1oo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
...
```

---

## ⭐ Acknowledgments

**Open-Source Libraries:**
- [RadioLib](https://github.com/jgromes/RadioLib) - Excellent LoRa abstraction layer by Jan Gromeš
- [GxEPD](https://github.com/ZinggJM/GxEPD) - E-Ink display driver by Jean-Marc Zingg
- [SparkFun u-blox GNSS](https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library) - Comprehensive GPS library
- [Adafruit Sensor Libraries](https://github.com/adafruit) - IMU, magnetometer, temp/humidity drivers
- [mbedTLS](https://github.com/Mbed-TLS/mbedtls) - Cryptographic library (AES-256, SHA-256)

**Community Feedback:**
- Tactical/survival forums for real-world testing scenarios
- Amateur radio operators for LoRa propagation insights
- Outdoor enthusiasts for UX improvements

**Special Thanks:**
- Espressif Systems for ESP32-S3 platform
- Semtech for LoRa technology
- All contributors and beta testers

---

<div align="center">

**Built with ❤️ for operators, adventurers, and survivalists**

*"When all other systems fail, GLYPH persists."*

[⬆ Back to Top](#-glyph-os)

</div>
