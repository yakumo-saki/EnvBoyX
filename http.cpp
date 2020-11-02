#include <Arduino.h>

void httplog(String msg) {
  Serial.println("HTTP: " + msg);
}

void http_setup() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", http_handle_ping );
  server.on ( "/", http_handle_data );
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop() {
  server.handleClient();
}

void http_handle_not_found() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  httplog(message);
  server.send ( 404, "text/plain", message );
}

void http_handle_data() {

  char buf[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  char temp[16], hum[16], pres[16];
  char lux[16], luxIr[16],ppm[16];
  char product[32];
  product_long.toCharArray(product, sizeof(product), 0);

  snprintf ( buf, sizeof buf,
    "{ \"product\": \"%s\", \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%02d\", \"temparature\": \"%s\", \"humidity\": \"%s\", \"pressure\": \"%s\",\"luminous\": \"%s\", \"luminousIr\": \"%s\", \"co2ppm\": \"%s\" }"
    , product, hr, min % 60, sec % 60, millis()
    , dtostrf(lastTemp, 0, 2, temp), dtostrf(lastHumidity, 0, 2, hum), dtostrf(lastPressure, 0, 2, pres)
    , dtostrf(lastLuxFull, 0, 0, lux), dtostrf(lastLuxIr, 0, 0, luxIr), dtostrf(lastPpm, 0, 0, ppm)
  );

  httplog("return json: " + String(buf));
  server.send ( 200, F("application/json"), buf );
}

void http_handle_ping() {

  char temp[100];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, sizeof temp,
    "{ \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%02d\" }\n"
    , hr, min % 60, sec % 60, millis()
  );

  httplog("return ping: " + String(temp));
  server.send ( 200, F("application/json"), temp );
  // digitalWrite(BUZZER_PIN, LOW );
}
