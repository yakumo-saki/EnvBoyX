## 書き込み方

### PlatformIO CLIをインストール

公式の説明は https://docs.platformio.org/en/latest/core/installation.html にあります。
`pio` コマンドが使えるようになっていればOKです。
macOS / Linux / Windows で使用可能です。

### バイナリの取得

GitHubの [releasesページ]([https://github.com/yakumo-saki/EnvBoyX/releases]) から`EnvBoy.tar.gz` を取得してください。
基本的には最新をお使いください。v38以前は `tar.gz`が提供されていません。

### アーカイブの展開

Linux / macOS: `tar cvf EnvBoyX.tar.gz`
Windows: アーカイバーを適宜ご使用ください。

### 書き込み

コマンドラインから

```
cd 展開先/EnvBoyX
pio run -t nobuild -t upload --disable-auto-clean
```

* `-t nobuild` ビルドしない（ソースが同梱されていないのでビルドできません）
* `-t upload` 書き込みする
* `--disable-auto-clean` ビルドディレクトリを自動的に削除しない（これをつけないと、ビルド済みバイナリが削除されます）

## Update via web browser

This method only for ESP32 and EnvBoyX v46.0 or above.

* download latest release from github.
* Go to http://[envboyx ip address or hostname]/update
* Upload and flash.

## 蛇足

以下は、必要でなければ読む必要はありません。

### ソースコードの修正が必要なパターン

以下に当てはまる場合は、ソースコードを取得して書き換えた後、再コンパイル→書き込みを行う必要があります。
カッコ内は書き換える対象のファイルです。

* ESP8266でI2Cに使用するピンがReadmeの記述と異なる場合 (global.cpp)
* ESP32でSPIに使用するピンがReadmeの記述と異なる場合 (platformio.ini)
* その他、独自機能を実装したい場合（是非PRください）