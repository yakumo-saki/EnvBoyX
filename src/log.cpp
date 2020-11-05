#include <Arduino.h>

void mainlog(String msg) {
  Serial.println("MAIN: " + msg);
}

void lpslog(String msg) {
  Serial.println("LPS22HB: " + msg);
}

void mhzlog(String msg) {
  Serial.println("MHZ19B: " + msg);
}

void tsllog(String msg) {
  Serial.println("TSL2561: " + msg);
}

void bmelog(String msg) {
  Serial.println("BME280: " + msg);
}

void amlog(String msg) {
  Serial.println("AM2320: " + msg);
}

void adtlog(String msg) {
  Serial.println("ADT7410: " + msg);
}
