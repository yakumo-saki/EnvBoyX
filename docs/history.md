# Version History

* BUGFIX: Bug fix
* FIX: Fix not bug, but not friendly behavior
* CHANGE: Change some behavior
* ADD: Add new function
* DROP: Delete some functions
* NOTE: other things

## v44.0: Untitled update

* 

## v43.0: Delta all update

* CHANGE: Espressif 32 framework 3.0 -> 3.1
* ADD: Delta display
* FIX: Alert config can't save (warning1.high, caution2.low&high)
* ADD: Config backup API /config/backup

## v42.0: Config update

* CHANGE: all API results are now JSON
* ADD: freeHeap to JSON, value of ESP.getFreeHeap() (update 15sec)
* ADD: Config GET / SET API
* ADD: Config COMMIT(SAVE) / REVERT API

## v41.0: MH-Z19B update

* ADD: API: MH-Z19B Auto Baseline Calibration ON/OFF
* ADD: API: MH-Z19B Zero Calibration

## v40.1: Bugfix release for v40

* FIX: SSD1306: Missing unit when no alerts
* CHANGE: Add minor version
* CHANGE: Delete startup first screen

## v40: Pressure Delta update

* FIX: CO2 ppm alert value is not good.
* ADD: Add POST /goto_setup API. 
* FIX: Delete unused source code.
* ADD: Add display invert (3sec) on setup mode (ST7789 / SSD1306)
* NOTE: platformio/espressif8266 @ 2.6.3
* FIX: MHZ19B RX, TX Pin settings are fixed to GPIO 14,0
* CHANGE: Stop prefixing "*" when co2ppm below 400 (use alerts for this purpose)
* FIX: Wait for reconfigure bar length invalid
* FIX: BME280: improve pressure precision (fix decimal values are truncated)
* ADD: Pressure delta
* CHANGE: ST7789: Normal: Dont show co2 when no co2 sensor.

## v39: SH1106 Support release

* CHANGE: CONFIG: Add SSD1306 / SH1106 switch
* CHANGE: HTTP: ESP32: Stop Async Web server. back to standard webserver to reduce code duplicate #42
* ADD: WiFi RSSI to JSON
* FIX: ST7789: T: header align

## v38: Small update release

* CHANGE: CONFIG: ESP32 now uses SPIFFS instead of LITTLEFS but no format or setup required (remove LITTLEFS to SPIFFS wrapper because of compile error)
* CHANGE: SSD1306: Move to U8G2 graphic library
* CHANGE: SSD1306: Lux unit "lx" to "Lx" (1 and l is very similer griph)
* CHANGE: SSD1306: Draw wait for reconfigure bar using graphics
* CHANGE: ST7789: Draw wait for reconfigure bar using graphics
* BUGFIX: CONFIG: coution 2 High value is not saved. (due to insufficient buffer)

## v37: Alert update.

* VERSION: 37. next version is v38.
* BUGFIX: Alert settings are not saved on ESP32.
* BUGFIX: Alert settings are not propery saved.
* CHANGE: Switch to squix78 -> thingpulse (same library)
* CONFIG: Shorten JSON keys about alerts(reconfig required)

## v3.6: Bugfix release.

* NOTE: LICENSE: EnvBoyX is now under APL 2.0 , Affected to all versions. (license terms are not shown before)
* BUGFIX: TSL2561 can't enabled
* CHANGE: Serial speed changed to 74880. (was 115200)
* BUGFIX: ping API returns invalid JSON
* BUGFIX: Display glitch when bigmode and show lux

## v3.5

* CHANGE: Config: version 9 (was 8) and using JSON format
* ADD: Config: Migration between versions

## v3.4

* CHANGE: Config: version 8 (was 6)
* BUGFIX: SSD1306 not shown in Setup Mode
* ADD: Default Brightness setting
* ADD: Screen Flip setting

## v3.3

* BUGFIX: MH-Z19B wrong message
* BUGFIX: Wrong uptime on JSON
* ADD: ST7789 Brightness change (via Web API)
* ADD: Watchdog timer (ESP32 only)
* CHANGE: Use TimerCall

## v3.2

* FIX: Avoid using delay
* ADD: I2C scan on startup.
* BUGFIX: ESP8266: I2C not working
* DROP: ESP8266: ST7789 is not supported (because of Pin config)

## v3.1

* ADD: vertical display mode(bigger font. ST7789 only)
* FIX: Refactoring

## v3.0

* ADD: ST7789 Support (T-Display)
* NOTE: ST7789 is initial support, some screens are simplifyed.

## v2.7: Display item change update

* CHANGE: Delete alive indicator "*" after EnvBoyX string.
* ADD: alive indicator. EnvBoyX's "X" character now blinks.
* CHANGE: prefix "IP:" before IP address
* ADD: mDNS name display. IP/mDNS display switches 3 sec interval.
* NOTE: Abort TFT display implementation.

## v2.6

* BUGFIX: Fix mDNS not working

## v2.5

* ADD: /display endpoint.

## v2.4

* ADD: /brightness endpoint.

## v2.3

* FIX: Unifing EnvBoyX (ESP8266) and EnvBoyX32 (ESP32) again.

## v2.2

* missing version due to bug.

## v2.1

* FIX: Split EnvBoyX (8266) and EnvBoyX32 (ESP32)

## v2.0

* FIX: Move Arduino IDE to Platform.IO

## before v2.0

* https://github.com/yakumo-saki/envboy
* https://github.com/yakumo-saki/EnvBoyMQTT
