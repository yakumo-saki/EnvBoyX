# EnvBoyX

Arduino environment monitor Unified version of Envboy and Envboy MQTT

## Moved to Platform IO since 2.0

last version of Arduino IDE, look `last_arduino_ide` tag.

# spec

## sensors

All sensors are optional.

* ESP8266
* BME280
* SSD1306
* TSL2561
* LPS22HB
* AM2302 (got wrong value at first time)

## how to use

* upload (use `pio run` )
* boot (as setup mode)
* connect to ESP8266 wifi (SSID ebx*******, no password)
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

# old version

these are only for old version

## how to use

* write sketch
* boot (as setup mode)
* connect to wifi (SSID is eb********)
* access http://192.168.4.1/ and setting.
* reset
* boot (as normal mode)
* enjoy!
