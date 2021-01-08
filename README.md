# EnvBoyX

Arduino environment monitor Unified version of Envboy and Envboy MQTT
To obtain release version, see vX.Y tag. (ex v2.6)
# spec

## target boards

### `ESP32 DevKitC`

Main target for now.
with SSD1306 I2C OLED (128x64px)

### `LilyGo TTGO T-DISPLAY`

ESP32 with ST7789V SPI 1.14 inch 240x135px display.

### ESP8266 boards

Sub target less tested, but maybe working.
with SSD1306 I2C OLED (128x64px)

## pin assign

### I2C

Defined at `global.cpp`

| TYPE  | SDA | SCL  | note             |
| ----- | ----| ---- | ---------------- |
|ESP32  |  21 | 22   | not configurable |
|ESP8266|   5 |  4   | not configurable |

### MH-Z19 Serial(UART)

Defined at `mhz19_uart.cpp`

| TYPE  | Rx  | Tx   | note                            |
| ----- | ----| ---- | ------------------------------- |
|ESP32  |  16 | 17   | HardwareSerial 2, configurable |
|ESP8266|  14 |  0   | SoftwareSerial , fixed          |

## sensors

All sensors are optional.

* BME280
* SSD1306
* TSL2561
* LPS22HB
* AM2302 (got wrong value at first time)

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

if SSD1306 is connected, sensor value are displayed in SSD1306

(v3.0)
ST7789 is supported. Activate on config screen.
In setup mode, no output on ST7789, it's normal.

(v3.2)
ST7789 support is now ESP32 only.
Maybe I can support ST7789 on ESP8266. but not in TODO.

### http server

All endpoints are using GET Method

| ENDPOINT      | description                |
| ------------- | -------------------------- |
| /             | returns sensor value as JSON.|
| /ping         | returns pong. |
| /brightness?value=<brightness> | set display brightness (0-255) 0 means OFF |
| /display?value=<1 or 0> | set display power (1 = ON / 0 = OFF) |

## how to write 

use esptools.py or PlatformIO IDE.

```
esptool.py write_flash 0x1000 filename.bin
```
Get binary from releases page.(not every version have binary file, sorry)

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

### v3.2

* Avoid using delay
* ADD: I2C scan on startup.
* BUGFIX: ESP8266: I2C not working
* DROP: ESP8266: ST7789 is not supported (because of Pin config)

### v3.1

* Add vertical display mode(bigger font. ST7789 only)
* Refactoring

### v3.0

* Add ST7789 Support (T-Display)
* ST7789 is initial support, some screens are simplifyed.

### v2.7

Display item change.

* Delete alive indicator "*" after EnvBoyX string.
* Add alive indicator. EnvBoyX's "X" character now blinks.
* Delete prefix "IP:" before IP address
* Add mDNS name display. IP/mDNS display switches 3 sec interval.
* Abort TFT display implementation.

### v2.6

Fix mDNS not working

### v2.5

Add /display endpoint.

### v2.4

Add /brightness endpoint.

### v2.3

Unifing EnvBoyX (ESP8266) and EnvBoyX32 (ESP32) again.

### v2.2

missing version due to bug.

### v2.1

Split EnvBoyX (8266) and EnvBoyX32 (ESP32)

### v2.0

Move to Platform.IO from Arduino IDE 