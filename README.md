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
In setup mode, no output.

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

## History

### v3.2

* Avoid using delay

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