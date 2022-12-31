# Version History

## v46.0: Big Usability improve update

* Config WebUI is always ON (normal and setup mode)
* Web Updater is added. (only on ESP32)

### UPGRADE NOTE

* Almost all API url are changed.
* API URLs are prefixed by /api/v1 is added. (exception: /ping /goto_setup).
* See [API Docs](api.md). 

### CHANGES

* ADD: Web config both on normal mode and setup mode
* BREAKING CHANGE: All API urls changed. 
* Deprecate: MQTT mode will be delete

## v45.0: Config update

### UPGRADE NOTE

### CHANGES

* ADD: AutoDimmer
* ADD: Config class (Internal mechanism) maybe consume 4KB memory.
* ADD: Config value validation (Setup web, Web API)
* CHANGE: platformio/espressif32 @ 3.2.0 (was 3.1.0)
* CHANGE: Config version v45 (was v44)
* NOTE: ESP32 freeHeap=238756 ESP8266 freeHeap=19184


## v44.0: Internal refactoring Update

### UPGRADE NOTE

* Some config are back to default value. (all alerts)
* If using before v42, update to v43 first. and get config backup using `http://[envboy]/config/backup` API

### CHANGES

* CHANGE: Config version 12 -> 44 (New config version scheme. same as EBX version)
* ADD: MHZ-19B Auto Baseline Correction ON/OFF at boot time
* ADD: Skip wait for reconfigure
* ADD: Factory reset API
* CHANGE: Config set API key about alerts are chenged
* FIX: Internal code clean up
* FIX: Config backup API result contains gabage
* FIX: mDNS: Call setInstanceName/addService before begin() (this prevent mdns listing) 
* FIX: Config set API aftercare is not executed

## v43.0: Delta all update

### UPGRADE NOTE

### CHANGES

* CHANGE: Espressif 32 framework 3.0 -> 3.1
* ADD: Delta display
* FIX: Alert config can't save (warning1.high, caution2.low&high)
* ADD: Config backup API /config/backup

## v42.0: Config update

### UPGRADE NOTE

### CHANGES

* CHANGE: all API results are now JSON
* ADD: freeHeap to JSON, value of ESP.getFreeHeap() (update 15sec)
* ADD: Config GET / SET API
* ADD: Config COMMIT(SAVE) / REVERT API

## v41.0: MH-Z19B update

### UPGRADE NOTE

### CHANGES

* ADD: API: MH-Z19B Auto Baseline Calibration ON/OFF
* ADD: API: MH-Z19B Zero Calibration

## v40.1: Bugfix release for v40

### UPGRADE NOTE

### CHANGES

* FIX: SSD1306: Missing unit when no alerts
* CHANGE: Add minor version
* CHANGE: Delete startup first screen

## v40: Pressure Delta update

### UPGRADE NOTE

### CHANGES

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

### UPGRADE NOTE

### CHANGES

* CHANGE: CONFIG: Add SSD1306 / SH1106 switch
* CHANGE: HTTP: ESP32: Stop Async Web server. back to standard webserver to reduce code duplicate #42
* ADD: WiFi RSSI to JSON
* FIX: ST7789: T: header align

## v38: Small update release

### UPGRADE NOTE

### CHANGES

* CHANGE: CONFIG: ESP32 now uses SPIFFS instead of LITTLEFS but no format or setup required (remove LITTLEFS to SPIFFS wrapper because of compile error)
* CHANGE: SSD1306: Move to U8G2 graphic library
* CHANGE: SSD1306: Lux unit "lx" to "Lx" (1 and l is very similer griph)
* CHANGE: SSD1306: Draw wait for reconfigure bar using graphics
* CHANGE: ST7789: Draw wait for reconfigure bar using graphics
* FIX: CONFIG: coution 2 High value is not saved. (due to insufficient buffer)

## v37: Alert update.

### UPGRADE NOTE

### CHANGES

* VERSION: 37. next version is v38.
* FIX: Alert settings are not saved on ESP32.
* FIX: Alert settings are not propery saved.
* CHANGE: Switch to squix78 -> thingpulse (same library)
* CONFIG: Shorten JSON keys about alerts(reconfig required)

## v3.6: Bugfix release.

### UPGRADE NOTE

### CHANGES

* NOTE: LICENSE: EnvBoyX is now under APL 2.0 , Affected to all versions. (license terms are not shown before)
* FIX: TSL2561 can't enabled
* CHANGE: Serial speed changed to 74880. (was 115200)
* FIX: ping API returns invalid JSON
* FIX: Display glitch when bigmode and show lux

## v3.5

### UPGRADE NOTE

### CHANGES

* CHANGE: Config: version 9 (was 8) and using JSON format
* ADD: Config: Migration between versions

## v3.4

### UPGRADE NOTE

### CHANGES

* CHANGE: Config: version 8 (was 6)
* FIX: SSD1306 not shown in Setup Mode
* ADD: Default Brightness setting
* ADD: Screen Flip setting

## v3.3

### UPGRADE NOTE

### CHANGES

* FIX: MH-Z19B wrong message
* FIX: Wrong uptime on JSON
* ADD: ST7789 Brightness change (via Web API)
* ADD: Watchdog timer (ESP32 only)
* CHANGE: Use TimerCall

## v3.2

### UPGRADE NOTE

### CHANGES

* FIX: Avoid using delay
* ADD: I2C scan on startup.
* FIX: ESP8266: I2C not working
* DROP: ESP8266: ST7789 is not supported (because of Pin config)

## v3.1

### UPGRADE NOTE

### CHANGES

* ADD: vertical display mode(bigger font. ST7789 only)
* FIX: Refactoring

## v3.0

### UPGRADE NOTE

### CHANGES

* ADD: ST7789 Support (T-Display)
* NOTE: ST7789 is initial support, some screens are simplifyed.

## v2.7: Display item change update

### UPGRADE NOTE

### CHANGES

* CHANGE: Delete alive indicator "*" after EnvBoyX string.
* ADD: alive indicator. EnvBoyX's "X" character now blinks.
* CHANGE: prefix "IP:" before IP address
* ADD: mDNS name display. IP/mDNS display switches 3 sec interval.
* NOTE: Abort TFT display implementation.

## v2.6

### UPGRADE NOTE

### CHANGES

* FIX: Fix mDNS not working

## v2.5

### UPGRADE NOTE

### CHANGES

* ADD: /display endpoint.

## v2.4

### UPGRADE NOTE

### CHANGES

* ADD: /brightness endpoint.

## v2.3

### UPGRADE NOTE

### CHANGES

* FIX: Unifing EnvBoyX (ESP8266) and EnvBoyX32 (ESP32) again.

## v2.2

### UPGRADE NOTE

### CHANGES

* missing version due to bug.

## v2.1

### UPGRADE NOTE

### CHANGES

* FIX: Split EnvBoyX (8266) and EnvBoyX32 (ESP32)

## v2.0

### UPGRADE NOTE

### CHANGES

* FIX: Move Arduino IDE to Platform.IO

## before v2.0

Changelog for EnvboyX before 2.0 is below;

* https://github.com/yakumo-saki/envboy
* https://github.com/yakumo-saki/EnvBoyMQTT
