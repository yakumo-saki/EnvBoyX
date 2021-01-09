#include <Arduino.h>

// WiFi
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "log.h"
#include "global.h"
#include "watchdog.h"

String wl_status_t_to_string(wl_status_t wl_stat) {
    if (wl_stat == WL_NO_SHIELD) {
        return "WL_NO_SHIELD";
    } else if (wl_stat == WL_IDLE_STATUS) {
        return "WL_IDLE_STATUS";
    } else if (wl_stat == WL_NO_SSID_AVAIL) {
        return "WL_NO_SSID_AVAIL";
    } else if (wl_stat == WL_SCAN_COMPLETED) {
        return "WL_SCAN_COMPLETED";
    } else if (wl_stat == WL_CONNECTED) {
        return "WL_CONNECTED";
    } else if (wl_stat == WL_CONNECT_FAILED) {
        return "WL_CONNECT_FAILED";
    } else if (wl_stat == WL_CONNECTION_LOST) {
        return "WL_CONNECTION_LOST";
    } else if (wl_stat == WL_DISCONNECTED) {
        return "WL_DISCONNECTED";
    }

    return "UNKNOWN_STATUS";

}

/**
 * WiFi接続する
 */
void make_sure_wifi_connected() {
  
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  watchdog_feed();

  wifilog("WiFi is down or not initialized. connecting");
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(false);

  delay(100);
  watchdog_feed();

  int retryCount = 0;
  wifilog("ssid " + config.ssid + " pass " + config.password);
  WiFi.begin(config.ssid.c_str(), config.password.c_str());
  
  delay(300);
  watchdog_feed();
  
  while (WiFi.status() != WL_CONNECTED) {

    watchdog_feed();
    delay(100);
    retryCount++;

    if (retryCount % 10 == 0) {
      wifilog("WiFI.status() = " + wl_status_t_to_string(WiFi.status()));
      wifilog("Still waiting for wifi connection");
    }
    if (retryCount % 50 == 0) {
      wifilog("Restarting WiFi");
      delay(100);
      wifilog("WiFi disconnect.");

      WiFi.disconnect();   
      delay(100);
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
      wifilog("RETRY connecting WiFi from start");
    }

    if (retryCount > 100) {
      wifilog("WiFi connect failure.");
      wifilog("Restarting");
      wifilog("ESP8266 note: must connect proper pins, otherwise device hangs");
      ESP.deepSleep(1 * 1000 * 1000);
      delay(10000);
    }
  }

  wifilog("WiFi (re) connected.");
  wifilog("IP address: " + WiFi.localIP().toString());
}


/**
 * WiFi AP モードを起動する
 */
void start_wifi_access_point() {
  byte mac[6];
  WiFi.macAddress(mac);

  // SSID は macaddress をSUFFIXする。前半はespressifのIDなので後半3つだけ
  config.ssid = "_SETUP_" + product_short + "_";
  for (int i = 3; i < 6; i++) {
    config.ssid += String(mac[i], HEX);
  }
  
  wifilog("SSID: " + config.ssid);
  // Serial.println("PASS: " + pass);

  /* You can remove the password parameter if you want the AP to be open. */
  // WiFi.softAP(ssid.c_str(), pass.c_str());
  WiFi.softAP(config.ssid.c_str());
  wifilog("WiFi AP Started. SSID=" + config.ssid);
}

/**
 * WiFiのIPアドレスを得る
 */
String get_wifi_ip_addr() {
  return WiFi.localIP().toString();
}


