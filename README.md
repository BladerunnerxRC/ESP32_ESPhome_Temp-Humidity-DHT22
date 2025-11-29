# ESP32_ESPhome_Temp-Humidity-DHT22

Configuration for an ESP32 running [ESPHome](https://esphome.io/) with a
DHT22 temperature/humidity sensor. The project exposes the sensor readings to
Home Assistant, mirrors them on an SSD1306 128x32 OLED display, and provides a few
helper automations such as Wi-Fi status and a restart trigger.

## Confirmed ESPHome Version

Confirmed to work with:

- (Also tested with ESPHome v2025.11.2)

## GPIO Assignments

| Pin    | Purpose                                     |
|--------|---------------------------------------------|
| GPIO2  | Onboard status LED (optional - internal)   |
| GPIO26 | DHT22 Data                                 |
| GPIO21 | I²C SDA (OLED Display)                     |
| GPIO22 | I²C SCL (OLED Display)                     |
| —      | WiFi Signal Sensor (internal / virtual)    |

The OLED display refreshes at the same interval as the DHT22 sensor updates (default: 30s),
and can be adjusted at runtime via Home Assistant.

## OLED Display Feature

An SSD1306 128x32 OLED display (I²C, address 0x3C) is supported. It shows:

- Live temperature (°F, converted from DHT22 °C)
- Humidity percentage
- Wi-Fi signal strength (dBm)
- Wi-Fi icon (only when connected)

The display refreshes at the same interval as the DHT22 sensor updates (default: 30s).
The Wi-Fi icon uses the `wifi.png` image, which must be uploaded to
`/homeassistant/esphome/images` on your Home Assistant server for correct compilation.

## Code Functionality Overview

- **Sensor Readings:**
  - DHT22 sensor on **GPIO26** provides temperature and humidity.
  - Readings are filtered using:
    - NaN filter
    - Clamp (temperature: -40–80°C, humidity: 0–100%)
    - Median filter
  - Temperature and humidity can be calibrated via Home Assistant using template `number` entities,
    which write to ESPHome `globals`.
  - Wi-Fi signal strength is reported using ESP32’s **internal** Wi-Fi RSSI sensor
    (no physical GPIO pin; exposed via `wifi_signal` platform).

- **Display:**
  - SSD1306 OLED on I²C (SDA=**GPIO21**, SCL=**GPIO22**).
  - Shows temperature (°F), humidity (%), Wi-Fi signal strength (dBm),
    and a Wi-Fi icon when connected.
  - Display updates on:
    - The regular sensor refresh interval (default: 30s).
    - Wi-Fi status changes (to immediately show/hide the Wi-Fi icon).

- **Wi-Fi Management & Status LED (GPIO2):**
  - Connects to Wi-Fi with a static IP configuration.
  - Onboard LED on **GPIO2** is used as a Wi-Fi status indicator:
    - **Wi-Fi connected:** LED is **off**.
    - **Wi-Fi disconnected / not yet connected:** LED **blinks** with a 1 second period
      (1s on, 1s off).
  - If the Wi-Fi connection is lost for more than 30 seconds, the device will automatically restart.
  - Fallback AP mode is available for recovery.

- **Home Assistant Integration:**
  - Exposes:
    - DHT22 temperature and humidity sensors.
    - Wi-Fi signal strength sensor.
    - Uptime sensor.
    - Calibration `number` entities for temperature and humidity.
    - Runtime-tunable `number` entities for:
      - DHT22 update interval (seconds).
      - Wi-Fi signal update interval (seconds).
    - A reboot switch.
  - Supports a remote reboot trigger via a Home Assistant `input_boolean`, with a debounce
    to prevent reboots if uptime is too short.

- **Runtime-Tunable Update Intervals:**
  - The DHT22 sensor and Wi-Fi RSSI sensor both run with `update_interval: never` and are
    driven by ESPHome `interval` blocks.
  - Intervals are controlled by `globals` and can be adjusted from Home Assistant using
    template `number` entities:
    - **DHT22 update interval** (default: 30s).
    - **Wi-Fi signal update interval** (default: 60s).

- **Other Features:**
  - OTA updates via ESPHome.
  - Encrypted API for Home Assistant.
  - Uptime sensor and a “Refresh Sensors” button to force immediate updates.

## Configuration Files

- All primary configuration is in `enviro-b2.yaml`.
- Secrets (Wi-Fi credentials, API keys, OTA passwords, etc.) are stored in `secrets.yaml`
  and referenced via `!secret` to keep sensitive data out of source control.
