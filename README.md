# ESP32_ESPhome_Temp-Humidity-DHT22

Configuration for an ESP32 running [ESPHome](https://esphome.io/) with a
DHT22 temperature/humidity sensor. The project exposes the sensor readings to
Home Assistant, mirrors them on an SSD1306 OLED display, and provides a few
helper automations such as Wi-Fi status and a restart trigger.

# Confirmed to work with ESPHome v2025.9.1

## GPIO Assignments

| Pin   | Purpose                     |
|-------|-----------------------------|
| GPIO2 | Onboard status LED          |
| GPIO21| SSD1306 OLED SDA            |
| GPIO22| SSD1306 OLED SCL            |
| GPIO26| DHT22 data pin              |

## OLED Display

An SSD1306 128x64 OLED connected over I²C shows the live temperature (in
Fahrenheit), humidity percentage, and Wi-Fi signal strength in dBm. The
display refreshes at the same interval as the DHT22 sensor updates.

The configuration is contained in `enviro-b2.yaml`. Secrets such as Wi-Fi
credentials are stored in `secrets.yaml` and referenced via `!secret` to keep
them out of source control.
