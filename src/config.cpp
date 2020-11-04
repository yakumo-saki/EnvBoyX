#include <Arduino.h>
#include <LittleFS.h>

#include "global.h"

void log(String msg) {
  Serial.println("MHZ19B: " + msg);
}

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir() {
  char cwdName[2];

  Serial.println(">>> LittleFS directory listing");

  strcpy(cwdName,"/");
  Dir dir = LittleFS.openDir(cwdName);
  
  while( dir.next()) {
    String fn, fs;
    fn = dir.fileName();
    fn.remove(0, 1);
    fs = String(dir.fileSize());
    Serial.println("<" + fn + "> size=" + fs);
  } // end while

  Serial.println("===");
}

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file() {
  File f2 = LittleFS.open(configured_file, "w");
  f2.println("ok");
  f2.close();
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file() {
    LittleFS.remove(configured_file);
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */ 
void save_config() {
  LittleFS.begin();

  // 設定ファイル
  File f = LittleFS.open(settings, "w");
  f.println(String(SETTING_ID));
  f.println(ssid);
  f.println(password);
  f.println(mDNS);
  f.println(opMode);
  f.println(use_mhz19b);
  f.println(mhz19b_pwmpin);
  f.println(mhz19b_txpin);
  f.println(mhz19b_rxpin);
  f.println(mqttBroker);
  f.println(mqttName); 
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
void read_config() {
  File f = LittleFS.open(settings, "r");
  String settingId = f.readStringUntil('\n'); // 使わない
  ssid = f.readStringUntil('\n');
  password = f.readStringUntil('\n');
  mDNS = f.readStringUntil('\n');  
  opMode = f.readStringUntil('\n');
  use_mhz19b = f.readStringUntil('\n');
  mhz19b_pwmpin = f.readStringUntil('\n');
  mhz19b_txpin = f.readStringUntil('\n');
  mhz19b_rxpin = f.readStringUntil('\n');
  mqttBroker = f.readStringUntil('\n');
  mqttName = f.readStringUntil('\n');  
  f.close();

  settingId.trim();
  ssid.trim();
  password.trim();
  mDNS.trim();
  opMode.trim();
  use_mhz19b.trim();
  mqttBroker.trim();
  mqttName.trim();

  log("S-ID: " + settingId);
  log("SSID: " + ssid);
  log("PASS: " + password);
  log("mDNS: " + mDNS);
  log("opMode: " + opMode);
  log("use MHZ19B: " + use_mhz19b);
  log("   PWM PIN: " + mhz19b_pwmpin);
  log("use MHZ19B: " + mhz19b_txpin);
  log("use MHZ19B: " + mhz19b_rxpin);
  log("MQTT Broker: " + mqttBroker);
  log("MQTT Name  : " + mqttName);  
}