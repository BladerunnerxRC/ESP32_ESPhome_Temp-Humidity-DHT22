# ESP32_ESPhome_Temp-Humidity-DHT22

Configuration for an ESP32 running [ESPHome](https://esphome.io/) with a
DHT22 temperature/humidity sensor. The project exposes the sensor readings to
Home Assistant, mirrors them on an SSD1306 128x32 OLED display, and provides a few
helper automations such as Wi-Fi status and a restart trigger.

# Confirmed to work with ESPHome v2025.9.3

## GPIO Assignments

| Pin   | Purpose                     |
|-------|-----------------------------|
| GPIO2  | Onboard status LED (optional - Internal) |
| GPI26  | DHT22 Data                 |
| GPI21  | I²C SDA (OLED Display)     |
| GPIO16 | WiFi Signal Sensor (internal) |
| GPI22  | I²C SCL (OLED Display)     |


display refreshes at the same interval as the DHT22 sensor updates.

## OLED Display Feature

An SSD1306 128x32 OLED display (I²C, address 0x3C) is supported. It shows:
- Live temperature (°F, converted from DHT22 Celsius)
- Humidity percentage
- Wi-Fi signal strength (dBm)
- Wi-Fi icon (only when connected)

The display refreshes at the same interval as the DHT22 sensor updates (default: 30s). The Wi-Fi icon uses the `wifi.png` image, which must be uploaded to `/homeassistant/esphome/images` on your Home Assistant server for correct compilation.

them out of source control.

## Code Functionality Overview

- **Sensor Readings:**
	- DHT22 sensor on GPIO4 provides temperature and humidity.
	- Readings are filtered (median, clamp, NaN filter) and can be calibrated via Home Assistant.
	- Wi-Fi signal strength is reported using ESP32's internal sensor (GPIO16).
- **Display:**
	- SSD1306 OLED (I²C: SDA=GPIO5, SCL=GPIO18) shows temperature, humidity, Wi-Fi dBm, and a Wi-Fi icon when connected.
	- Display updates on sensor refresh or Wi-Fi status change.
- **Wi-Fi Management:**
	- Connects to Wi-Fi with static IP; shows status on OLED and onboard LED (GPIO2).
	- If disconnected for 30s, device restarts automatically.
	- Fallback AP mode is available for recovery.
- **Home Assistant Integration:**
	- Exposes all sensors, calibration numbers, and a reboot switch.
	- Allows remote reboot via Home Assistant input_boolean.
- **Other Features:**
	- OTA updates and encrypted API for Home Assistant.
	- Uptime sensor, refresh button, and runtime-tunable update intervals.

All configuration is in `enviro-b2.yaml`. Secrets (Wi-Fi, API keys) are in `secrets.yaml` and referenced via `!secret`.

![CodeRabbit Pull Request Reviews](https://img.shields.io/coderabbit/prs/github/BladerunnerxRC/ESP32_ESPhome_Temp-Humidity-DHT22?utm_source=oss&utm_medium=github&utm_campaign=BladerunnerxRC%2FESP32_ESPhome_Temp-Humidity-DHT22&labelColor=171717&color=FF570A&link=https%3A%2F%2Fcoderabbit.ai&label=CodeRabbit+Reviews)
