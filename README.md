# EnvBoyX

Arduino environment monitor Unified version of Envboy and Envboy MQTT
To obtain release version, see vX.Y tag. (ex v2.6)
# spec

## target boards

* ESP32 DevKitC
* (not well tested, compile only) ESP8266 boards

## pin assign

### I2C

Defined at `global.cpp`

| TYPE  | SDA | SCL  |
| ----- | ----| ---- |
|ESP32  |  21 | 22   |
|ESP8266|   5 |  4   |

### MH-Z19 Serial(UART)

Defined at `mhz19_uart.cpp`

| TYPE  | Rx  | Tx  |
| ----- | ----| ---- |
|ESP32  |  16 | 17   |
|ESP8266|  14 |  0   |

## sensors

All sensors are optional.

* BME280
* SSD1306
* TSL2561
* LPS22HB
* AM2302 (got wrong value at first time)

## how to use

* upload (use `pio run` )
* boot (as setup mode)
* connect to ESP wifi (SSID ebx*******, no password)
* access http://192.168.4.1/ and setting.
* reset
* boot (as normal mode)
* enjoy!

## functions

### read sensor value and display

if SSD1306 is connected, sensor value are displayed in SSD1306

### http server

All endpoints are using GET Method

| ENDPOINT      | description                |
| ------------- | -------------------------- |
| /             | returns sensor value as JSON.|
| /ping         | returns pong. |
| /brightness?value=<brightness> | set display brightness (0-255) 0 means OFF |
| /display?value=<1 or 0> | set display power (1 = ON / 0 = OFF) |

## History

### v2.7 (WIP)

Refactoring

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