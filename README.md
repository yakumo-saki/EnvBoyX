# EnvBoyX

Arduino environment monitor for ESP32 / ESP8266 based boards.

## Getting started

* [Japanese: 書き込み方法（日本語）](docs/how_to_write-ja.md)
* [How to write](docs/how_to_write-en.md)

## Branches

| name   | usage |
| ------ | ---------------------------------- |
| master | latest Release                     |
| devel  | currently developing unstable code |

# images

## Default output image on TTGO T-Display

![Default Output Image](docs/EnvBoyX_Horizontal.jpg)

## Vertical Output Image on TTGO T-Display

![Vertical Output Image](docs/EnvBoyX_Vertical.jpg)

# spec

## target boards

* ESP32 based boards
* ESP8266 based boards

## functions

* HTTP Server
* MQTT publisher
* Display
* mDNS

### note

* ESP8266 mDNS is unstable.
* ESP8266 MH-Z19B is unstable
	// Init I2C Serial
	init_i2c(I2C_SDA, I2C_SCL);
	// Init I2C Serial
	init_i2c(I2C_SDA, I2C_SCL);

## pin assign

### I2C

Defined at `global.cpp`

| TYPE  | SDA | SCL  | note             |
| ----- | ----| ---- | ---------------- |
|ESP32  |  21 | 22   | not configurable |
|ESP8266|   5 |  4   | not configurable |

### MH-Z19 Serial(UART)

Defined at `mhz19_uart.cpp`

| TYPE  | Rx  | Tx   | note                                          |
| ----- | ----| ---- | --------------------------------------------- |
|ESP32  |  32 | 33   | HardwareSerial 2, configurable via web config |
|ESP8266|  14 |  0   | SoftwareSerial , fixed                        |

## sensors and devices

All are optional.

* BME280 (Temperature, Humidity, Pressure sensor)
* TSL2561 (luminous sensor)
* LPS22HB (Pressure sensor)
* AM2302 (Temperature sensor, got wrong value at first time)
* MH-Z19B (CO2 sensor UART mode)
* SSD1306 (I2C OLED 128x64px)
* ST7789 (SPI TFT 240x135px)

## how to use

### first time

* upload (use `pio run` or esptool.py etc)
* boot (as setup mode)
* connect to ESP wifi (SSID ebx_******* (MAC adderess of your ESP), no password)
* Open web browser and open http://192.168.4.1/ and setup.
* reset(use hardware button or disconnect power plug)
* boot (as normal mode)
* enjoy!

### change setting

* Poweroff or reset on waiting for resetting screen.
 
### version up

* upload
* boot (if config version changed, automatically start on setup mode.)

## functions

### Display

* SSD1306: Auto detect
* SH1106: Usable but need setting on setting mode, otherwise you see line on screen edge
* ST7789: Must configure in web config

(v3.0)
ST7789 is supported. Activate on config screen.
In setup mode, no output on ST7789, it's normal.

(v3.2)
ST7789 support is now ESP32 only.
Maybe I can support ST7789 on ESP8266. but not in TODO.

### http server

EnvBoyX has http API.

| METHOD | ENDPOINT      | RETURN | description                |
| ------ | ------------- | -------------------------- |
| GET | /             | JSON | sensor value as JSON.|
| GET | /ping         | TEXT | returns pong. |
| GET | /stastics     | JSON | stastics JSON |
| GET | /brightness?value=<brightness> | TEXT | set display brightness (0-255) 0 means OFF |
| GET | /display?value=<1 or 0> | TEXT | set display power (1 = ON / 0 = OFF) |
| POST | /goto_setup | TEXT | Go to setup mode at next boot |

NOTE: There is no reboot API, because of security reason.

#### note

* brightness and display power function is NOT WORKING in ST7789 (v3.0)

## how to write 

use esptools.py or PlatformIO IDE.

```
esptool.py write_flash 0x1000 filename.bin
```

* Get binary from releases page.

## Setups I am using

* T-Display with BME280, MHZ-19B
* NodeMCU V3 V2 ESP32 with SSD1306, BME280, MH-Z19B
* NodeMCU V3 ESP8266 with BME280, MH-Z19B

## Boards I tested (at least once)

### ESP32

* LilyGO TTGO T-DISPLAY (ESP32 and ST7789 TFT)
* NodeMCU V3 V2 ESP32

### ESP8266

* D-Duino (ESP8266 and ST1306 OLED board)
* WeMos D1mini
* NodeMCU V3

## Version History

* BUGFIX: Bug fix
* FIX: Fix not bug, but not friendly behavior
* CHANGE: Change some behavior
* ADD: Add new function
* DROP: Delete some functions
* NOTE: other things

### v40

* FIX: CO2 ppm alert value is not good.
* ADD: Add POST /goto_setup API. 
* FIX: Delete unused source code.
* ADD: Add display invert (3sec) on setup mode (ST7789 / SSD1306)
* NOTE: platformio/espressif8266 @ 2.6.3
* FIX: MHZ19B RX, TX Pin settings are fixed to GPIO 14,0
* CHANGE: Stop prefixing "*" when co2ppm below 400 (use alerts for this purpose)
* FIX: Wait for reconfigure bar length invalid
* FIX: BME280: improve pressure precision (fix decimal values are truncated)

### v39

* CHANGE: CONFIG: Add SSD1306 / SH1106 switch
* CHANGE: HTTP: ESP32: Stop Async Web server. back to standard webserver to reduce code duplicate #42
* ADD: WiFi RSSI to JSON
* FIX: ST7789: T: header align

### v38

* CHANGE: CONFIG: ESP32 now uses SPIFFS instead of LITTLEFS but no format or setup required (remove LITTLEFS to SPIFFS wrapper because of compile error)
* CHANGE: SSD1306: Move to U8G2 graphic library
* CHANGE: SSD1306: Lux unit "lx" to "Lx" (1 and l is very similer griph)
* CHANGE: SSD1306: Draw wait for reconfigure bar using graphics
* CHANGE: ST7789: Draw wait for reconfigure bar using graphics
* BUGFIX: CONFIG: coution 2 High value is not saved. (due to insufficient buffer)

### v37

* VERSION: 37. next version is v38.
* BUGFIX: Alert settings are not saved on ESP32.
* BUGFIX: Alert settings are not propery saved.
* CHANGE: Switch to squix78 -> thingpulse (same library)
* CONFIG: Shorten JSON keys about alerts(reconfig required)

### v3.6

* LICENSE: EnvBoyX is now under APL 2.0 , Affected to all versions. (license terms are not shown before)
* BUGFIX: TSL2561 can't enabled
* CHANGE: Serial speed changed to 74880. (was 115200)
* BUGFIX: ping API returns invalid JSON
* BUGFIX: Display glitch when bigmode and show lux

### v3.5

* CHANGE: Config: version 9 (was 8) and using JSON format
* ADD: Config: Migration between versions

### v3.4

* CHANGE: Config: version 8 (was 6)
* BUGFIX: SSD1306 not shown in Setup Mode
* ADD: Default Brightness setting
* ADD: Screen Flip setting

### v3.3

* BUGFIX: MH-Z19B wrong message
* BUGFIX: Wrong uptime on JSON
* ADD: ST7789 Brightness change (via Web API)
* ADD: Watchdog timer (ESP32 only)
* CHANGE: Use TimerCall

### v3.2

* FIX: Avoid using delay
* ADD: I2C scan on startup.
* BUGFIX: ESP8266: I2C not working
* DROP: ESP8266: ST7789 is not supported (because of Pin config)

### v3.1

* ADD: vertical display mode(bigger font. ST7789 only)
* FIX: Refactoring

### v3.0

* ADD: ST7789 Support (T-Display)
* NOTE: ST7789 is initial support, some screens are simplifyed.

### v2.7: Display item change update

* CHANGE: Delete alive indicator "*" after EnvBoyX string.
* ADD: alive indicator. EnvBoyX's "X" character now blinks.
* CHANGE: prefix "IP:" before IP address
* ADD: mDNS name display. IP/mDNS display switches 3 sec interval.
* NOTE: Abort TFT display implementation.

### v2.6

* BUGFIX: Fix mDNS not working

### v2.5

* ADD: /display endpoint.

### v2.4

* ADD: /brightness endpoint.

### v2.3

* FIX: Unifing EnvBoyX (ESP8266) and EnvBoyX32 (ESP32) again.

### v2.2

* missing version due to bug.

### v2.1

* FIX: Split EnvBoyX (8266) and EnvBoyX32 (ESP32)

### v2.0

* FIX: Move Arduino IDE to Platform.IO

### before v2.0

* https://github.com/yakumo-saki/envboy
* https://github.com/yakumo-saki/EnvBoyMQTT
