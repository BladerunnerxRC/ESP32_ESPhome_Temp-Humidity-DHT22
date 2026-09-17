# ESP32_ESPhome_Temp-Humidity-DHT22

Configuration for an ESP32 running [ESPHome](https://esphome.io/) with a
DHT22 temperature/humidity sensor. The project exposes the sensor readings to
Home Assistant, mirrors them on an SSD1306 128x32 OLED display, and provides a few
helper automations such as Wi-Fi status and a restart trigger.

## Release

- Current config release: **v1.6.0** (`enviro-b2.yaml`)

## Confirmed ESPHome Version

Confirmed to work with:

- ESPHome v2026.9.0 (user-provided device logs, 2026-09-16)

## GPIO Assignments

| Pin    | Purpose                                    |
|--------|--------------------------------------------|
| GPIO2  | Onboard status LED (optional - internal)   |
| GPIO26 | DHT22 Data                                 |
| GPIO21 | I²C SDA (OLED Display)                     |
| GPIO22 | I²C SCL (OLED Display)                     |
| —      | WiFi Signal Sensor (internal / virtual)    |

DHT22 sampling defaults to 30 seconds and is adjustable in Home Assistant.
The OLED redraws when readings change and every five seconds for freshness checks.

## OLED Display Feature

An SSD1306 128x32 OLED display (I²C, address 0x3C) is supported. It shows:

- Live temperature (°F, converted from DHT22 °C)
- Humidity percentage
- Wi-Fi signal strength (dBm)
- Wi-Fi icon (only when connected)

The display follows sensor publications and refreshes every five seconds for freshness checks.
Keep the image at `images/wifi.png` and the font at
`fonts/Roboto-Regular.ttf` relative to the YAML on the build host.
Both assets are local, so compilation does not need a Google Fonts download.

## Code Functionality Overview

- **Sensor Readings:**
  - DHT22 sensor on **GPIO26** provides temperature and humidity.
  - Readings are filtered using:
    - Reject nonfinite and out-of-range raw samples (-40–80°C, 0–100%)
    - Median filter (window size: 5, publish every valid sample)
    - Calibration followed by humidity saturation to 0–100%
  - Temperature and humidity can be calibrated via Home Assistant using template `number` entities,
    which write to ESPHome `globals`.
  - Wi-Fi signal strength is reported using ESP32's **internal** Wi-Fi RSSI sensor
    (no physical GPIO pin; exposed via `wifi_signal` platform).

- **Display:**
  - SSD1306 OLED on I²C (SDA=**GPIO21**, SCL=**GPIO22**).
  - Shows temperature (°F), humidity (%), Wi-Fi signal strength (dBm),
    and a Wi-Fi icon when connected.
  - Display updates on:
    - Published readings, coalesced into one redraw after 50ms.
    - Five-second freshness checks; stale values are displayed as --.
    - Wi-Fi status changes (to immediately show/hide the Wi-Fi icon).

- **Wi-Fi Management & Status LED (GPIO2):**
  - Connects to Wi-Fi with a static IP configuration.
  - Onboard LED on **GPIO2** indicates connection state with three distinct patterns:
    - **Fully connected** (Wi-Fi + HA API): LED is **off**.
    - **Wi-Fi connected, no HA client**: LED blinks **3 long — 1 fast — 3 long** (~7s cycle).
    - **No Wi-Fi**: LED **blinks** with a 1 second period (1s on, 1s off).
  - LED state is driven entirely by the 1s interval loop reading `g_wifi_connected`
    and a state-subscribing API client check. Logger-only connections do not count as HA.
    The LED switch is internal; it cannot be controlled from Home Assistant.
  - Native Wi-Fi recovery uses a 15-minute timeout; it does not reboot in fallback AP mode.
  - The fallback AP and captive portal open after 90 seconds.
  - API disconnection does not reboot the device; it keeps measuring during HA maintenance.

- **Home Assistant Integration:**
  - Exposes:
    - DHT22 temperature and humidity sensors (`entity_category: none` — primary sensors).
    - Wi-Fi signal strength sensor.
    - Uptime sensor (`entity_category: diagnostic`).
    - Connection status binary sensor (`entity_category: diagnostic`).
    - Calibration `number` entities for temperature (°C) and humidity (%) — `entity_category: config`.
    - Runtime-tunable `number` entities for:
      - DHT22 update interval (seconds) — `entity_category: config`.
      - Wi-Fi signal update interval (seconds) — `entity_category: config`.
    - A restart button (`entity_category: config`).
    - A "Refresh Sensors" button with a guarded DHT read after two seconds.
    - A "DHT Readings Stale" problem entity; HA retains the last good sensor values.
  - All configuration and diagnostic entities are grouped separately in the HA device page.
  - Replace old restart-switch/helper automations with the new button.
  - Connection notifications should be implemented in HA after Connection Status turns on.
  - See [connection recovery and HA migration](docs/connectivity.md).

- **Runtime-Tunable Update Intervals:**
  - Native DHT and Wi-Fi pollers start after restored settings are loaded.
  - Existing globals are the only persisted settings; template numbers mirror them.
  - Interval changes replace the native polling intervals, avoiding custom timestamp arithmetic.
  - Manual refresh pauses DHT polling and enforces two-second spacing around the read.
  - See [sensor behavior and acceptance checks](docs/sensors.md).

- **Runtime Efficiency:**
  - ESP32 framework set to `version: recommended` for stable, ESPHome-validated builds.
  - `minimum_chip_revision: "3.1"` matches the confirmed ESP32 revision.
  - `sram1_as_iram: true` enables 40 KB of additional instruction RAM; the device logs confirm bootloader support.
  - Operational logging uses INFO; UART remains disabled (`baud_rate: 0`).
  - Wi-Fi RSSI update path is skipped while disconnected.

- **Other Features:**
  - Required encrypted OTA using the existing API key (ESPHome 2026.9.0+).
  - Follow the [OTA migration procedure](docs/ota.md) before installing on older firmware.
  - Encrypted API for Home Assistant.
  - Uptime sensor and a "Refresh Sensors" button to force immediate updates.

## Configuration Files

Copy the YAML and supporting files to the build host, preserving the subfolders
shown below.

When building with **ESPHome Builder in Home Assistant**, place `enviro_helpers.h`
in an `includes/` subfolder beside `enviro-b2.yaml`. If your editor shows the configuration
folder as `/homeassistant/esphome`, use:

```text
/homeassistant/esphome/
├── enviro-b2.yaml
├── includes/
│   └── enviro_helpers.h
├── images/
│   └── wifi.png
└── fonts/
    └── Roboto-Regular.ttf
```

Some environments expose this folder as `/config/esphome`; use whichever folder
contains your device YAML. Copy the header using File editor, Studio Code Server,
or a Samba share, then save it and build again. The YAML references the header
relative to its own folder:

```yaml
esphome:
  includes:
    - includes/enviro_helpers.h
```

The font reference is `file: "fonts/Roboto-Regular.ttf"`, relative to the YAML.
The image reference is `file: "images/wifi.png"`; place the image in
`/homeassistant/esphome/images/`.
Keep your existing font in `/homeassistant/esphome/fonts/`; do not replace this
relative reference with an absolute `/homeassistant` or `/config` path. Builder
may show `/config/esphome` inside its container while your editor shows
`/homeassistant/esphome`.

- All primary configuration is in `enviro-b2.yaml`.
- Secrets (Wi-Fi credentials, API keys, OTA passwords, etc.) are stored in `secrets.yaml`
  and referenced via `!secret` to keep sensitive data out of source control.

## Development validation

Run `python scripts/validate_config.py` with ESPHome 2026.9.0 installed.
The script builds an isolated copy with dummy secrets; it never uploads firmware.
Use `--generate-only` for schema validation and C++ generation without compiling.
