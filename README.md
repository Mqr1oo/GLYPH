GLYPH OS

Beyond the Signal: Tactical Communication & Navigation

An ESP32-based hybrid OS built for tactical operations, extreme trekking, and survival. Delivers military-grade encrypted LoRa messaging, offline GNSS navigation, and real-time team tracking on an ultra-low-power E-Ink display.

📑 Table of Contents

🔥 Core Capabilities

🥷 Stealth & Survivability

🧰 Hardware & Components

💻 Installation & Setup

🆘 Emergency & Connectivity

🔥 Core Capabilities

🗺️ Offline Tactical Map & Navigation

Live Breadcrumbs: Real-time route drawing with advanced GPS noise filtering (only records true movement > 20cm).

KML Overlays: Load pre-planned tactical routes directly from the SD card.

Live Telemetry: Pinpoint accurate distance and altitude tracking, featuring 3 dynamic zoom levels.

📡 Encrypted LoRa Messenger

Military-Grade Security: AES-256-CBC encryption. Keys are dynamically generated via SHA-256 hashing of your Team Name to prevent unauthorized interception.

Invisible Geolocation: Automatically attaches your hidden GPS coordinates to every transmitted message.

Smart Input: Type directly using the rugged 3-button physical interface, or pair via Bluetooth to use your smartphone's keyboard.

👥 Team Radar

Live Squad Tracking: Monitor up to 5 team members simultaneously on your screen.

Auto-Extraction: Automatically parses coordinates from incoming LoRa chatter to update squad positions on your radar without manual input.

Status Monitoring: Built-in "Last Seen" timestamps calculate the age and reliability of teammate locations.

🥷 Stealth & Survivability

GLYPH OS is built to survive extended missions with a dynamic power engine:

STEALTH Mode: Complete radio silence (Zero TX/RX emissions). GPS updates only every 50 seconds. Electronically undetectable.

ECO Mode: Balances long-range hiking with 10-second GPS intervals and active LoRa listening for maximum battery efficiency.

Anti-Ghosting E-Ink: Zero-glare, ultra-low power consumption display providing crisp visibility in direct sunlight.

🧰 Hardware & Components

To build your own GLYPH device, you will need the following core modules:

Component

Specification

Description

MCU

ESP32-S3 Dual-Core

Handles heavy cryptography and UI processing.

Display

2.13" E-Ink (GxEPD2)

High-contrast, sunlight-readable screen.

Radio

SX1262 LoRa

868MHz/915MHz with adjustable power (up to 22dBm).

GNSS

u-blox GNSS (I2C)

Fast time-to-first-fix and high accuracy.

Sensors

LSM6DSOX + LIS3MDL

6-Axis IMU and tilt-compensated compass.

💻 Installation & Setup

We recommend using PlatformIO for compiling and uploading the firmware.

# 1. Clone the repository
git clone https://github.com/your-username/glyph-os.git

# 2. Navigate to the directory
cd glyph-os

# 3. Build and upload to the ESP32
pio run -t upload


Note: Ensure you have the RadioLib and GxEPD2 libraries installed if you choose to use the Arduino IDE instead of PlatformIO.

🆘 Emergency & Connectivity

Instant SOS Broadcast: A dedicated hardware button combo (B + C) instantly broadcasts a continuous SOS signal at maximum LoRa range (Spread Factor 12).

BLE Tactical Modem: Connect via WebBluetooth to your smartphone. The device acts as an offline gateway, pushing telemetry and message notifications directly to your mobile browser—no cell network required.

Black Box Logging: Automatically logs all movement as .KML routes and full telemetry data as .CSV to the SD card for post-mission analysis (natively compatible with ATAK and Google Earth).
