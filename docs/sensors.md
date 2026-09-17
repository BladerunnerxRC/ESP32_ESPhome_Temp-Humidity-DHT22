# Sensor timing, freshness, and display updates

DHT and Wi-Fi native pollers are started after persisted settings restore. The
existing global IDs retain their stored values; template numbers mirror them on
boot instead of saving a second copy. Interval changes replace the native pollers.
No custom seconds-based millis scheduler remains. Wi-Fi signal reads while offline
are handled by ESPHome's wifi_signal component.

Both median filters use five samples and publish every valid sample. The window
still smooths changes. Raw values outside -40..80 C or 0..100% and nonfinite values
are rejected. Humidity is bounded to 0..100% after calibration.

The OLED redraws 50ms after published readings, immediately for Wi-Fi changes,
and every five seconds to expire each reading independently.
When either DHT reading exceeds three selected sampling intervals, DHT Readings
Stale turns on and the corresponding OLED value becomes --. HA sensor entities
retain their last good values; use the problem entity to gate HA automations.
Both fresh readings clear the diagnostic. No data at boot is treated as stale.

Refresh Sensors waits two seconds before DHT sampling and two afterwards; requests
during that script are ignored. Calibration changes use the same guarded refresh.
This avoids back-to-back DHT reads. A median may still contain older samples.

## Acceptance checks

- Default interval: confirm valid values publish every 30s, not every 150s.
- Change intervals to 10s and 3600s, reboot, and verify the selected values and
  sampling cadence restore; OLED follows publications at either interval.
- Disconnect the DHT: old values stay in HA, the problem entity turns on after
  three intervals, and the OLED hides each expired reading. Reconnect and recover.
- Set humidity offset to +10 near saturation and -10 near zero; readings remain
  within 0..100%. Temperature calibration remains in Celsius.
- Press refresh rapidly and change calibration; verify no unguarded rapid reads.
- Confirm the Wi-Fi row hides the cached RSSI while disconnected.

Run the C++ tests in tests/sensor_behavior.cpp with a host C++ compiler. They cover
validity boundaries, calibration saturation, stale expiry, and millis wraparound.
Build with ESPHome 2026.9.0 before device testing. Copy enviro_helpers.h alongside
the YAML and display assets to Device Builder.
