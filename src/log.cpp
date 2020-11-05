#include <Arduino.h>

void real_log(String msgString, String prefixString) {
  char log[120];
  char prefix[10];
  char msg[100];
  prefixString.toCharArray(prefix, sizeof prefix);
  msgString.toCharArray(msg, sizeof msg);
  snprintf(log, sizeof log, "%08lu %-10s: %s", millis(),  prefix, msg);
  Serial.println(log);
}

void mainlog(String msg) {
  real_log(msg, "MAIN");
}

void lpslog(String msg) {
  real_log(msg, "LPS22HB");
}

void mhzlog(String msg) {
  real_log(msg, "MHZ19B");
}

void tsllog(String msg) {
  real_log(msg, "TSL2561");
}

void bmelog(String msg) {
  real_log(msg, "BME280");
}

void amlog(String msg) {
  real_log(msg, "AM2320");
}

void adtlog(String msg) {
  real_log(msg, "ADT7410");
}

void cfglog(String msg) {
  real_log(msg, "CONFIG");
}

void httplog(String msg) {
  real_log(msg, "HTTP");
}

void sectionlog(String msg) {
  Serial.println("");
  real_log(msg, "========");
  Serial.println("");
}