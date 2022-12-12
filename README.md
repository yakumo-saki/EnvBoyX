# EnvBoyX

Arduino environment monitor for ESP32 / ESP8266 based boards.

## All of documents

* [書き込み方法（日本語）](docs/how_to_write.ja.md)
* [How to write](docs/how_to_write.en.md)

View document on read the docs
https://envboyx.readthedocs.io/en/latest/

# images

## Default output image on TTGO T-Display

![Default Output Image](docs/imgs/EnvBoyX_Horizontal.jpg)

## Vertical Output Image on TTGO T-Display

![Vertical Output Image](docs/imgs/EnvBoyX_Vertical.jpg)

# overview

## target boards

* ESP32 based boards
* ESP8266 based boards

## functions

* Show sensor output on display
* HTTP Json endpoint for getting sensor output 
* Highly configurable and flexible
* Supports many sensors

## sensors and devices

All are optional.

* BME280 (Temperature, Humidity, Pressure sensor)
* TSL2561 (luminous sensor)
* LPS22HB (Pressure sensor)
* AM2302 (Temperature sensor, got wrong value at first time)
* MH-Z19B (CO2 sensor UART mode)
* SSD1306 (I2C OLED 128x64px)
* ST7789 (SPI TFT 240x135px)

# specs

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

## how to use

### first time

* Read getting started of this document.

### change setting

* Poweroff or reset on waiting for resetting screen.
* (v42) Use Config API
 
### version up

* Same as first time
* After version up, EnvBoy may be automatically boots up in SETUP MODE if config parameter is added.

## functions

### Display

* SSD1306: Auto detect
* SH1106: Usable but need setting on setting mode, otherwise you see line on screen edge
* ST7789: Must configure in web config

### HTTP Web API

EnvBoyX has HTTP web API.
[API Docs](docs/api.md) for details.

# appendix

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

## build note

* Use python script to store some html, css
* Script is stored in build_script
* Script is automatically executed by platformio
* See platform.io [env] extra_script for details.

## Version History

see [Version History](docs/history.md) for details.
