# API Documentations

## HTTP Web API

EnvBoyX has http web API.

| since | METHOD | ENDPOINT      | RETURN | description                |
| ------| ------ | ------------- | -------------------------- | -------|
| 2.0   | GET | /             | JSON | sensor value as JSON.|
| 2.0   | GET | /ping         | TEXT | returns pong. |
| 39.0  | GET | /stastics     | JSON | stastics JSON |
| 40.0  | POST | /goto_setup  | JSON | Go to setup mode at next boot |
| 3.4   | GET | /api/v1/brightness   | JSON | value=0-255 set display brightness (0-255) 0 means OFF (adHoc, not config) |
| 3.4   | GET | /api/v1/display      | JSON | set display power (1 = ON / 0 = OFF) |
| 41.0  | POST | /api/v1/mhz19b/abc  | JSON | value=1 ON value=0 OFF |
| 41.0  | POST | /api/v1/mhz19b/zeroCalibration | TEXT | __DANGER__ use if you know what you are doing. see below |
| 42.0  | GET  | /api/v1/config                 | JSON | Get config |
| 42.0  | POST | /api/v1/config                 | JSON | Change config |
| 42.0  | POST | /api/v1/config/commit          | JSON | Save config |
| 42.0  | POST | /api/v1/config/revert          | JSON | Revert to last saved config |
| 43.0  | GET | /api/v1/config/backup          | TEXT | Get current running config on curl command line |
| 44.0  | POST | /api/v1/config/factory-reset   | JSON | Delete ALL configs | 

NOTE: There is no reboot API, because of security reason.

## /api/v1/mhz19b/zeroCalibration

This method may break your MHZ19B accuracy.
By datasheet, place mh-z19b in under 400ppm for at least 20 minutes.

## /api/v1/config API

This API changes config (same thing in setup mode)
But some paramters are not changeable.

### parameters 

names: `config_names.cpp`
values: `config_values.cpp`

| name | possible values | need restart | note                       |
|------|-----------------| -- | ------------------------ |
| mqttName   | string | y | *1 mqttClientName. only affects in MQTT Mode |
| mqttBroker | IP address | y | *1 MQTT Broker IP addr. only affects in MQTT Mode |
| mhz19b | "yes_uart", "no" | y | MHZ19B mode |
| mhz19bRxPin | integer | y | MHZ19B UART RX Pin no, only available on ESP32 | 
| mhz19bTxPin | integer | y | MHZ19B UART TX Pin no, only available on ESP32 | 
| mhz19bPwmPin | integer | y | not supported |
| st7789 | "yes", "no" | y | Use ST7789 or not. only on ESP32 |
| st7789Mode | "st7789_BIG", "st7789_normal" | n | ST7789 display mode |
| displayFlip | "yes", "no" | n | Flip display or not |
| displayWaitForReconfigure | "skip", "on" | n | Skip on boot wait for reconfigure screen |
| displayAutoDimLux | integer | n | Minimum lux to display off |
| displayAutoDimWaitSec | integer | n | Wait time to display off |
| opMode | "always", "mqtt" | y | Operation mode |
| mDNS | string | n | mDNS hostname |
| co2Alerts  | {alerts} | n | See alerts section |
| humiAlerts | {alerts} | n | See alerts section |
| luxAlerts  | {alerts} | n | See alerts section |
| presAlerts | {alerts} | n | See alerts section |

*1 config api is not available in MQTT mode.

#### Alert parameters

Alert config keys structure:

`<kind>.<level>.<low_or_high>`

##### v44 and above

kind = [co2alerts | humiAlerts | luxAlerts | presAlerts]
level_and_no = [warn1 | warn2 | caut1 | caut2]
low_or_high = [L | H]

example: co2alerts.warn1.L

##### before v44

kind = [co2alerts | humiAlerts | luxAlerts | presAlerts]
level_and_no = [warning1 | warning2 | caution1 | caution2]
low_or_high = [low | high]
example: co2alerts.warning1.low 
#### cURL example

```
curl -s -X POST -d "displayFlip=yes&displayBrightness=50" "http://ebx32.local/api/v1/config"
```

## HTTP Web API cURL examples

These are example of calling API

### Simple get API

```
curl http://[envboy IP or mDNShostname.local]/api/v1/getdata

{"product":"EnvBoyX","uptime":"01:18:02","uptimeMills":4682994,"temparature":"28.60","humidity":"35.00","pressure":"976.81","luminous":"16","luminousIr":"2","co2ppm":"-999","co2ppmAccuracy":"","rssi":-12}
```

### GET with parameter API

```
curl http://[envboy IP or mDNShostname.local]/brightness?value=100`
OK
```

### POST without parameter API

```
$ curl -X POST http://[envboy IP or mDNShostname.local]/goto_setup
OK
```

### Appendix API urls until v45

This information is old until v45.

| since | METHOD | ENDPOINT      | RETURN | description                |
| ------| ------ | ------------- | -------------------------- | -------|
| 2.0   | GET | /             | JSON | sensor value as JSON.|
| 2.0   | GET | /ping         | TEXT | returns pong. |
| 39.0  | GET | /stastics     | JSON | stastics JSON |
| 3.4   | GET | /brightness   | JSON | value=0-255 set display brightness (0-255) 0 means OFF (adHoc, not config) |
| 3.4   | GET | /display      | JSON | set display power (1 = ON / 0 = OFF) |
| 40.0  | POST | /goto_setup  | JSON | Go to setup mode at next boot |
| 41.0  | POST | /mhz19b/abc  | JSON | value=1 ON value=0 OFF |
| 41.0  | POST | /mhz19b/zeroCalibration | TEXT | __DANGER__ Use if you know what you are doing |
| 42.0  | GET  | /config                 | JSON | Get config |
| 42.0  | POST | /config                 | JSON | Change config |
| 42.0  | POST | /config/commit          | JSON | Save config |
| 42.0  | POST | /config/revert          | JSON | Revert to last saved config |
| 43.0  | GET | /config/backup          | TEXT | Get current running config on curl command line |
| 44.0  | POST | /config/factory-reset   | JSON | Delete ALL configs | 

NOTE: There is no reboot API, because of security reason.
