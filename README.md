# EnvBoyX32

Arduino environment monitor Unified version of Envboy and Envboy MQTT

# spec

## target boards

* ESP32 DevKitC
* (not well tested) ESP8266 boards

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
* connect to ESP32 wifi (SSID ebx*******, no password)
* access http://192.168.4.1/ and setting.
* reset
* boot (as normal mode)
* enjoy!

## functions

### read sensor value and display

if SSD1306 is connected, sensor value are displayed in SSD1306

### http server

endpoints

* / : returns sensor value as JSON.
* /ping : returns pong.
