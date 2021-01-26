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

void displog(String msg) {
  real_log(msg, "DISPLAY");
}

void mdnslog(String msg) {
  real_log(msg, "mDNS");
}

void wifilog(String msg) {
  real_log(msg, "WiFi");
}

void i2clog(String msg) {
  real_log(msg, "I2C");
}
void ssdlog(String msg) {
  real_log(msg, "SSD1306");
}
void stlog(String msg) {
  real_log(msg, "ST7789");
}

void statlog(String msg) {
  real_log(msg, "STAT");
}

void debuglog(String msg) {
  real_log(msg, "DEBUG");
}

void sectionlog(String msg) {
  Serial.println("");
  real_log(msg, "========");
  Serial.println("");
}

String get_i2c_error_message(byte code) {
    
    if (code == 0) {
        return "code 0:success (not error)";
    } else if (code == 1) {
        return "code 1:data too long to fit in transmit buffer";
    } else if (code == 2) {
        return "code 2:received NACK on transmit of address";
    } else if (code == 3) {
        return "code 3:received NACK on transmit of data";
    } else if (code == 4) {
        return "code 4:other error";
    }
       
    return "code " + String(code) + ":unknown error";
}
