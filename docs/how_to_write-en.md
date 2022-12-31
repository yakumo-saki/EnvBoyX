## How to Write

### Install PlatformIO CLI

Install PlatformIO CLI 
https://docs.platformio.org/en/latest/core/installation.html

PlatformIO CLI is available for macOS / Linux / Windows platforms.

### Get EnvBoyX binary

Get release archive from [GitHub releases]([https://github.com/yakumo-saki/EnvBoyX/releases])
Recommend newest version.

### Extract 

macOS / Linux `tar cvf EnvBoyX.tar.gz`
Windows: use your favorite tool.

### Write

```
cd (extracted_dir)/EnvBoyX
pio run -t nobuild -t upload --disable-auto-clean
```

* `-t nobuild` no build
* `-t upload` Do write
* `--disable-auto-clean` Don't delete firmware.bin

## Update via web browser

This method only for ESP32 and EnvBoyX v46.0 or above.

* download latest release from github.
* Go to http://[envboyx ip address or hostname]/update
* Upload and flash.