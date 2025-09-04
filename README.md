# ESP32_ESPhome_Temp-Humidity-DHT22

Configuration for an ESP32 running [ESPHome](https://esphome.io/) with a
DHT22 temperature/humidity sensor. The project exposes the sensor readings to
Home Assistant and provides a few helper automations such as Wi-Fi status and
a restart trigger.

# Confirmed to work with ESPHome v2025.8.3

[Changelog](https://esphome.io/changelog/2025.8.0/#release-202581---august-25)

## GPIO Assignments

| Pin   | Purpose                     |
|-------|-----------------------------|
| GPIO2 | Onboard status LED          |
| GPIO26| DHT22 data pin              |

The configuration is contained in `enviro-b2.yaml`. Secrets such as Wi-Fi
credentials are stored in `secrets.yaml` and referenced via `!secret` to keep
them out of source control.
