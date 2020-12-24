#include <Arduino.h>

// WiFi
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// mDNS
#ifdef ESP32
#include <ESPmDNS.h>
#elif defined(ESP8266)
#include <ESP8266mDNS.h>
#endif

#include "log.h"
#include "global.h"

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
  
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(false);

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  mainlog("WiFi is down or not initialized. connecting");
  WiFi.disconnect();
  delay(100);

  int retryCount = 0;
  mainlog("ssid " + ssid + " pass " + password);
  WiFi.begin(ssid.c_str(), password.c_str());
  delay(300);

  while (WiFi.status() != WL_CONNECTED) {

    mainlog("WiFI.status() = " + wl_status_t_to_string(WiFi.status()));

    delay(100);
    retryCount++;

    if (retryCount % 10 == 0) {
      mainlog("Waiting for wifi connection");
    }
    if (retryCount % 100 == 0) {
      delay(100);
      WiFi.disconnect();   
      delay(100);
      WiFi.begin(ssid.c_str(), password.c_str());
      mainlog("RETRY connecting WiFi from start");
    }

    if (retryCount > 300) {
      mainlog("WiFi connect failure. restarting");
      ESP.deepSleep(1 * 1000 * 1000);
      delay(10000);
    }
  }

  mainlog("WiFi (re) connected.");
  mainlog("IP address: " + WiFi.localIP().toString());
}


/**
 * WiFi AP モードを起動する
 */
void start_wifi_access_point() {
  byte mac[6];
  WiFi.macAddress(mac);

  // SSID は macaddress をSUFFIXする
  ssid = product_short + "_";
  for (int i = 0; i < 6; i++) {
    ssid += String(mac[i], HEX);
  }
  
  mainlog("SSID: " + ssid);
  // Serial.println("PASS: " + pass);

  /* You can remove the password parameter if you want the AP to be open. */
  // WiFi.softAP(ssid.c_str(), pass.c_str());
  WiFi.softAP(ssid.c_str());
  mainlog("WiFi AP Started. SSID=" + ssid);
}

/**
 * WiFiのIPアドレスを得る
 */
String get_wifi_ip_addr() {
  return WiFi.localIP().toString();
}


bool start_mdns(String name) {
  char n[name.length() + 1];
  name.toCharArray(n, sizeof n);

  if (!MDNS.begin(n)) {
    mdnslog("Error setting up MDNS responder!");
    return false;
  }
  mdnslog("mDNS responder started");
  return true;
}