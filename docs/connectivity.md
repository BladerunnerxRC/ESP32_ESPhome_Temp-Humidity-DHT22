# Connection recovery and Home Assistant migration

Wi-Fi uses native reconnection with a 15-minute reboot timeout, except while the
fallback AP is active. The AP opens after 90 seconds and provides a captive portal.
API disconnection does not reboot the device: local sensing and display continue
during Home Assistant maintenance. This intentionally removes the API reboot
watchdog; Wi-Fi recovery remains enabled.

Connection Status requires a client subscribed to entity states. Logger-only
connections do not turn the LED off. Other state-subscribing clients can satisfy
this check; it is not an authenticated Home Assistant identity test. The three
existing LED patterns remain, but the LED switch is now internal.

## Home Assistant changes

- Replace calls to the old restart switch or `input_boolean.reboot_enviro_b2`
  with `button.press` targeting the new Reboot_ENVIRO-B2 button. Select the actual
  entity ID in your installation. The obsolete helper can be removed after all
  automations are migrated.
- The device no longer sends a notification before HA connects. If desired,
  create an HA automation triggered when this device's Connection Status changes
  from off to on for five seconds, calling `persistent_notification.create`.
  Set a fixed notification ID to replace the previous notification.
- INFO operational logging replaces WARN-only logging. UART logging stays off.
- Unused Home Assistant time synchronization is removed.

## Device acceptance checks

1. Disconnect HA while keeping Wi-Fi up; sensing must continue past 15 minutes.
2. Connect only a log client; the no-HA LED pattern must remain active.
3. Restore HA; Connection Status turns on and the LED turns off.
4. Disable the access point; verify no 30-second reboot and fallback AP at 90s.
5. Restore Wi-Fi; verify automatic reconnection and recovery portal shutdown.
6. Press the new restart button once and confirm a single clean restart.

Sources: https://esphome.io/components/wifi/ and https://esphome.io/components/api/
