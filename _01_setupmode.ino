
/**
 * 初期化
 */
void setup_setupmode() {
  // ファイルシステム初期化
  setup_server();
  disp_setup_startup_screen();
}

void loop_setupmode() {
  server.handleClient();
}

/**
 * GET 設定画面
 */
void handle_get_root() {
  String html = "";
  html += "<!doctype html>";
  html += "<html>";
  html += "<head>";
  html += "<title>" + product + " setting</title>";
  html += "<meta charset='UTF-8'>";
  html += "<style>";
  html += "  input[type='text'] { width:200px }";
  html += "  input[type='submit'] { width:200px;}";
  html += "</style>";
  html += "</head>";
  html += "<h1>" + product + " Settings</h1>";
  html += "<form method='post'>";
  html += "  <br>";
  html += "  接続先WiFiの情報です。面倒ですが正しく設定して下さい。<br>";
  html += "  2.4GHz帯のみ対応しています。<br>";
  html += "  <input type='text' name='ssid' placeholder='ssid' autofocus><br>";
  html += "  <input type='text' name='pass' placeholder='pass'><br>";
  html += "  mDNS(Rendezvous) の名前です。LAN内の他の端末等と重複しないようにして下さい。<br>";
  html += "  ハイフン、アンダースコアを使用すると名前解決に失敗する可能性があるため非推奨です。<br>";
  html += "  <input type='text' name='mdnsname' placeholder='mdnsname' value='" + ssid + "'><br>";
  html += "  <br>";
  html += "  動作モード<br>";
  html += "  <br>";
  html += "  <input type='radio' name='opmode' value='always' id='opmode_always' checked><label for='opmode_always'>常時起動モード（測定値常時表示）</label></label>";
  html += "  <input type='radio' name='opmode' value='mqtt' id='opmode_mqtt'><label for='opmode_mqtt'>MQTTモード（間欠動作・ディープスリープ）</label><br>";
  html += "  <br>";
  html += "  MH-Z19B CO2センサー有無（金色のセンサー）<br>";  
  html += "  <input type='radio' name='use_mhz19b' value='" + String(USE_NO)  + "' id='mhz19b_no' checked><label for='mhz19b_no'>使用しない（通常はこちら）</label>";
  html += "  <input type='radio' name='use_mhz19b' value='" + String(USE_YES) + "' id='mhz19b_yes'><label for='mhz19b_yes'>使用する（センサーが接続されている場合）</label><br>";
  html += "  <br>";
  html += "  ＜MQTTモード専用。それ以外では入力不要です＞<br>";
  html += "  MQTTブローカーのIPアドレスです。ホスト名も可能ですが、mDNSは使用出来ません。<br>";
  html += "  <input type='text' name='mqttbroker' placeholder='mqttbroker' value='""'><br>";
  html += "  <br>";
  html += "  ＜MQTTモード専用。それ以外では入力不要です＞<br>";
  html += "  MQTT名です。クライアント名とトピック名に使用されます。<br>";
  html += "  <input type='text' name='mqttname' placeholder='mqttname' value='""'><br>";
  html += "  <br>";
  html += "  <input type='submit' value='設定する'><br>";
  html += "</form>";
  html += "<br>";
  html += "<hr>";
  html += product_long + ", Copyright 2018-2019 ziomatrix.org.";
  html += "</html>";

  server.send(200, "text/html", html);
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  String mdnsname = server.arg("mdnsname");
  String opmode = server.arg("opmode");
  String mhz19b = server.arg("use_mhz19b");
  String mqttbroker = server.arg("mqttbroker");
  String mqttname = server.arg("mqttname");

  SPIFFS.begin();

  // 設定ファイル
  File f = SPIFFS.open(settings, "w");
  f.println(String(SETTING_ID));
  f.println(ssid);
  f.println(pass);
  f.println(mdnsname);
  f.println(opmode);
  f.println(mhz19b); 
  f.println(mqttbroker);
  f.println(mqttname); 
  f.close();

  // 設定済みフラグファイル
  File f2 = SPIFFS.open(configured_file, "w");
  f2.println("ok");
  f2.close();

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>" + product + " setting done. please restart.</title>";
  html += "<meta charset='UTF-8'>";
  html += "<style>";
  html += "  input { width:200px; }";
  html += "</style>";
  html += "</head>";

  html += "<h1>" + product + " setting done</h1>";
  if (opmode == "always") {
    html += "動作モード：常時稼働モード<br>";    
    html += "SSID " + ssid + "<br>";
    html += "PASS " + pass + "<br>";
    html += "mDNS " + mdnsname + "<br>";
    if (mhz19b == USE_YES) {
      html += "MHZ19B を使用する<br>";     
    } else if (mhz19b == USE_NO) {
      html += "MHZ19B を使用しない、または接続されていない<br>";     
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + use_mhz19b + "<br>";           
    }

  } else if (opmode == "mqtt") {
    html += "動作モード：MQTT間欠動作モード<br>";
    html += "SSID " + ssid + "<br>";
    html += "PASS " + pass + "<br>";
    html += "mDNS " + mdnsname + "<br>";
    html += "MQTT broker " + mqttbroker + "<br>";
    html += "MQTT name   " + mqttname + "<br>";
    if (mhz19b == USE_YES) {
      html += "MHZ19B を使用する<br>";     
    } else if (mhz19b == USE_NO) {
      html += "MHZ19B を使用しない、または接続されていない<br>";     
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + use_mhz19b + "<br>";           
    }
  } else {
    html += "動作モード：不明" + opmode + "<br>";    
  }

  html += "<br>";
  html += "Restart (power off then power on) to use above setting.<br>";
  html += "設定が完了しました。リセットボタンを押して再起動して下さい。<br>";
  html += "<br>";
  html += "<a href='/'>setting again</a>";
  html += "</html>";
  server.send(200, "text/html", html);
}

/**
 * 初期化(設定用Webサーバモード)
 */
void setup_server() {
  byte mac[6];
  WiFi.macAddress(mac);

  // SSID は macaddress をSUFFIXする
  ssid = product_short + "_";
  for (int i = 0; i < 6; i++) {
    ssid += String(mac[i], HEX);
  }
  
  Serial.println("SSID: " + ssid);
  // Serial.println("PASS: " + pass);

  /* You can remove the password parameter if you want the AP to be open. */
  // WiFi.softAP(ssid.c_str(), pass.c_str());
  WiFi.softAP(ssid.c_str());

  server.on("/", HTTP_GET, handle_get_root);
  server.on("/", HTTP_POST, handle_post_root);
  server.begin();
  Serial.println("HTTP server started.");
}
