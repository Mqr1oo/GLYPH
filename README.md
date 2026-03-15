🛡️ GLYPH: Offline Maps. Online Safety.

"Start your journey, track your path, and always find your way back home."

GLYPH is a professional-grade decentralized tactical navigation and communication framework. It is engineered for explorers, hikers, and search-and-rescue teams operating in "Black Zones"—environments where GSM signals are non-existent and traditional connectivity fails.

🧭 The Mission: Resilient Navigation

In remote wilderness, losing your orientation isn't just an inconvenience—it's a critical risk. GLYPH solves this by providing a reliable Breadcrumb Tracking system and a robust Off-Grid Communication link that operates independently of cellular towers or expensive satellite subscriptions.

Core Philosophy

Breadcrumb Reliability: High-precision path logging from the very first step.

Secure LoRa Mesh: Encrypted peer-to-peer messaging for team coordination.

Tactical Hardware: Sunlight-readable E-Ink display with extreme battery longevity.

Zero Infrastructure: Works anywhere on the planet, completely decentralized.

🚀 Key Modules (Tactical OS v9.50)

1. Advanced Breadcrumb Tracking 🗺️

Unlike standard GPS trackers that suffer from "signal bounce," GLYPH uses an IMU-Driven Anti-Drift Filter.

Movement Awareness: Using the LSM6DS3TR-C, the system knows if you are actually walking or standing still.

Anti-Drift Logic: If no steps are detected for 60 seconds, the system ignores GPS jitter, ensuring your traveled distance is 100% accurate.

KML Visualization: Routes are exported as 3D .KML files, ready for Google Earth analysis.

2. Off-Grid Communication 📡

GLYPH turns your device into a tactical radio node.

Dual-Band Support: Configurable for 868MHz (EU) or 915MHz (US).

Team Radar: Every transmitted message invisibly carries your GPS coordinates. Teammates can see your exact distance and bearing on their HUD.

Secure Link: Instant switching between PUBLIC (unencrypted) and SECURE (AES-style encryption based on your Team Name).

3. Bionic Sleep Engine 🔋

Energy management is critical for survival.

80MHz Optimization: The ESP32-S3 CPU is clocked at 80MHz to balance a fluid UI with ultra-low power draw.

Wake-on-Motion: The device enters deep sleep when stationary but wakes up instantly the moment you pick it up.

🛠️ Hardware & Bill of Materials (BOM)

The project utilizes high-end modular components for laboratory-grade reliability.

Total Build Cost: €140 - €190

Category

Component

Part Number / Specification

Mainboard

LilyGo T3 S3 E-Paper

LoRa 868MHz / 915MHz

GNSS Module

SparkFun GPS-15210

SAM-M8Q + Qwiic PCB Antenna

Motion/IMU

LSM6DS3TR-C + LIS3MDL

6-Axis Accel/Gyro + Magnetometer

Environmental

Adafruit 4885

Sensirion SHT40 (Temp/Hum)

Interface

Arduino Modulino

Tactile Buttons (A, B, C)

Connectivity

SparkFun PRT-17259

5x Flexible Qwiic Cables (5cm)

Power

Samsung INR18650-35E

3400mAh Li-Ion Cell

Enclosure

3D Printed

TPU (Shockproof) + PETG/PETG-CF

📊 Operational Profiles

Mode

CPU Speed

GPS Rate

BLE / Radio

NORMAL

80 MHz

1.0 Hz (1s)

ALL ACTIVE

ECO

40 MHz

0.1 Hz (10s)

Radio Active / BLE Off

STEALTH

40 MHz

0.1 Hz (10s)

ALL RADIOS OFF

📱 The Command Center: Tactical Web Dashboard

GLYPH bridges the gap between rugged hardware and modern smartphones. Our custom web interface serves as your interactive ground station, allowing for real-time data visualization and fluid communication.

1. Unified Launch Hub

By accessing the main URL, you enter the GLYPH Hub—a central gateway that lets you choose between the Tactical Dashboard for operations or the Firmware Flasher for maintenance.

2. Zero-Latency Wireless Teletype

Forget tiny buttons for complex messages.

Real-time Input: Use your smartphone's native keyboard to type long messages.

Instant Sync: As you type on your phone, the text appears character-by-character on the GLYPH E-Ink display, thanks to a high-speed BLE UART link.

One-Tap TX: Send your message to the LoRa mesh instantly from your screen.

3. Live Vital HUD

The dashboard provides a high-contrast, military-grade HUD that displays live data fetched directly from the hardware sensors:

GPS Fix Status: Real-time latitude and longitude updates.

Power Diagnostics: Accurate battery percentage monitoring.

Secure Link Feed: A specialized chat window that color-codes incoming (RX) and outgoing (TX) Lora messages, keeping your operational log organized.

4. PWA Installation (Offline Mode)

The dashboard is a Progressive Web App. Once you open the link with internet, you can "Add to Home Screen." It will then save itself to your phone's memory, allowing you to open the full interface in the middle of a forest with Zero Cellular Data.

Engineered by MQR1OO // Tactical Connectivity for the Modern Explorer
