# Required encrypted OTA

ESPHome 2026.9.0 can require encrypted firmware uploads using the existing API
key. Keep that key unchanged. The OTA password is replaced by `encryption: {}`.

The supplied ENVIRO-B2 logs show firmware 2026.9.0 and
`Encryption: offered, plaintext accepted`, so this device is ready for the change.
That evidence describes the running device, not a hardware test of this PR.

## Rollout

1. For older devices, first install 2026.9.0 retaining the API key and OTA password.
2. Confirm `Encryption: offered, plaintext accepted` in the device logs.
3. Apply this PR and install with ESPHome 2026.9.0 or newer.
4. Confirm `Encryption: required`, API reconnection, and successful sensor readings.

An initial upload to older firmware fails if required encryption is enabled too
early. Password and required encryption cannot be combined. Key changes are not
part of this migration; preserve the current key for uploads and Home Assistant.
To roll back application changes, build the older application configuration while
retaining encrypted OTA and the current key, using a compatible ESPHome release.
Lost-key recovery requires a separate recovery route such as serial flashing.

Source: https://esphome.io/components/ota/esphome/
