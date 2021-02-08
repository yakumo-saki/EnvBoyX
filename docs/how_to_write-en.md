## How to Write

### Install PlatformIO CLI

Install PlatformIO CLI 
https://docs.platformio.org/en/latest/core/installation.html に

PlatformIO CLI is available for macOS / Linux / Windows platforms.

### Get EnvBoyX binary

Get release archive from [GitHub releasesページ]([https://github.com/yakumo-saki/EnvBoyX/releases])
Recommend newest version. Older than v38 not have archive file.

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
