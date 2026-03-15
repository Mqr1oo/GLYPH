GLYPH OS - Beyond the Signal

Version 10.05 - Native BLE Integration

GLYPH OS is a lightweight, high-performance tactical operating system designed for the LilyGo T3 S3 E-Paper platform. It provides off-grid communication, real-time navigation, and environmental monitoring in a low-power, high-visibility package.

By leveraging WebBluetooth, GLYPH OS synchronizes seamlessly with a mobile web interface without requiring dedicated apps, allowing for secure field operations using only a browser.

🛰️ Key Features

Tactical Mapping: KML overlay support via SD Card with breadcrumb tracking and distance calculation.

Off-Grid Messaging: Long-range LoRa communication with support for Secure (AES-like XOR) and Public channels.

Team Radar: Real-time location sharing and distance tracking for up to 5 teammates.

Environmental Suite: Precision sensing (Temp/Hum) with active SHT40 heater control for anti-condensation in extreme environments.

Ultra-Low Power: Bionic Sleep engine with Deep/Light sleep modes and IMU-based wake-up.

Native BLE UART: Professional integration with the Nordic UART Service (NUS) for real-time telemetry and remote control.

Multilingual Interface: Support for 10 languages with a UI optimized for tactical brevity (< 25 characters).

🛠️ Bill of Materials (BOM)

Total Estimated Build Cost: €140 - €190

⚡ Solderless Build: The coolest part about this project is that you can assemble the entire system in a single afternoon. No soldering is required—everything is connected via the Qwiic/Modulino plug-and-play ecosystem.

Mainboard: LilyGo T3 S3 E-Paper (ESP32-S3, LoRa 868/915MHz, 2.13" E-Ink) — €45

GNSS Module: SparkFun GPS-15210 (u-blox SAM-M8Q + Qwiic PCB Antenna) — €40

Motion/IMU: LSM6DS3TR-C (6-Axis Accel/Gyro + Magnetometer) — €15

Environmental: Adafruit 4885 (Sensirion SHT40 High Precision) — €10

Interface: Arduino Modulino (3x Tactical Mechanical Buttons) — €20

Connectivity: SparkFun PRT-17259 (5x Flexible Qwiic Cables 5cm) — €10

Power: Samsung INR18650-35E (3400mAh High-Capacity Li-Ion Cell) — €10

Enclosure: Tactical Shell (3D Printed TPU Shockproof + PETG-CF) — €20

🚀 Future Roadmap

[ ] LoRa Mesh Integration: Implementation of multi-hop mesh networking for extended range.

[ ] ATAK Forwarder: Bridge functionality to forward LoRa packets to ATAK/CivTAK via BLE.

[ ] On-Device GIS: Vector map rendering directly from SD card (simplified .map files).

[ ] Solar Harvesting Logic: Dynamic CPU scaling based on real-time solar charging input.

[ ] Environmental AI: Barometric trend analysis for localized storm prediction.

💻 Technical Setup

Hardware: Connect sensors via the Qwiic I2C bus. Ensure the Radio and E-Paper pins are configured as per the glyph_os.ino header.

Firmware: Flash using Arduino IDE or PlatformIO. Required libraries: RadioLib, GxEPD, U8g2_for_Adafruit_GFX, SparkFun_u-blox_GNSS.

Interface: Open the GLYPH Web Controller (HTML/JS) in a WebBluetooth compatible browser (Chrome/Edge/Bluefy).

Connect: Click "Connect", select GLYPH, and start transmitting.

🛡️ Security

GLYPH OS uses a salt-based XOR encryption for "Secure Mode" messaging. While effective for tactical obfuscation in the field, it is intended for non-critical coordination. For mission-critical security, future updates will explore hardware-accelerated AES-256.

Developed by GLYPH Labs. Efficiency. Resilience. Stealth.
