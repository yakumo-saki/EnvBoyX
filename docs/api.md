# API Documentations

## HTTP Web API

EnvBoyX has http web API.

| since | METHOD | ENDPOINT      | RETURN | description                |
| ------| ------ | ------------- | -------------------------- | -------|
| 2.0   | GET | /             | JSON | sensor value as JSON.|
| 2.0   | GET | /ping         | TEXT | returns pong. |
| 39.0  | GET | /stastics     | JSON | stastics JSON |
| 3.4   | GET | /brightness   | JSON | value=0-255 set display brightness (0-255) 0 means OFF |
| 3.4   | GET | /display      | JSON | set display power (1 = ON / 0 = OFF) |
| 40.0  | POST | /goto_setup  | JSON | Go to setup mode at next boot |
| 41.0  | POST | /mhz19b/abc  | JSON | value=1 ON value=0 OFF |
| 41.0  | POST | /mhz19b/zeroCalibration | TEXT | __DANGER__ Use if you know what you are doing |
| 42.0  | GET  | /config                 | JSON | Get config |
| 42.0  | POST | /config                 | JSON | Change config |
| 42.0  | POST | /config/commit          | JSON | Save config |
| 42.0  | POST | /config/revert          | JSON | Revert to last saved config |

NOTE: There is no reboot API, because of security reason.

## HTTP Web API cURL examples

These are example of calling API

##### Simple get API

```
curl http://[envboy IP or mDNShostname.local]/
{"product":"EnvBoyX","uptime":"01:18:02","uptimeMills":4682994,"temparature":"28.60","humidity":"35.00","pressure":"976.81","luminous":"16","luminousIr":"2","co2ppm":"-999","co2ppmAccuracy":"","rssi":-12}
```

##### GET with parameter API

```
curl http://[envboy IP or mDNShostname.local]/brightness?value=100`
OK
```

##### POST without parameter API

```
$ curl -X POST http://[envboy IP or mDNShostname.local]/goto_setup
OK
```
