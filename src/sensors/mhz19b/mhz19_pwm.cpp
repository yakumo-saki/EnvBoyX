#include <Arduino.h>

#include "ConfigClass.h"

#include "log.h"
#include "global.h"
#include "sensors/mhz19_main.h"

// PWM
int CO2_IN = -1;

// 0 ~ 1004までなのでintでOK
int mhz_high_ms = 0;
int mhz_low_ms = 0;

/**
 * worst expected execution time 2008ms + alpha
 */
void mhz_read_pwm_ms() {

  const unsigned long TIMEOUT_MS = 2500;

  unsigned long high_start_ms = 0;
  unsigned long low_start_ms = 0;

  bool is_ready = false;

  String WAIT_TGT = "";

//  bool log_wait_for_low  = false;
//  bool log_wait_for_high = false;
  bool log_ignore_high = false;

  unsigned long timeout_start_ms = millis();
  unsigned long timeout_end_ms = timeout_start_ms + TIMEOUT_MS;
  mhzlog("read_mhz_pwm start " + String(timeout_start_ms));

  while (millis() < timeout_end_ms) {
    int pwmValue = digitalRead(CO2_IN);

#ifdef ESP8266
    ESP.wdtFeed();
#endif

    // Serial.println(pwmValue);
    
    if (pwmValue == LOW) {
      if (!is_ready) {
        // LOWになったので次のHIGHから計測開始
        is_ready = true;
        WAIT_TGT = "HIGH";
        mhzlog("LOW wait for first HIGH ms=" + String(millis()) );
      } else if (WAIT_TGT == "LOW") {
        // 計測開始後、HIGH→LOW になった時(HIGH計測終了）。次はLOW計測
        unsigned long end_ms = millis();
        low_start_ms = end_ms;
        WAIT_TGT = "HIGH2";
        unsigned long elapsed = end_ms - high_start_ms;
        mhz_high_ms = elapsed;

        mhzlog("LOW measure HIGH done. next: measure LOW. waiting HIGH ms=" + String(low_start_ms) );     
        continue;
      } else {
        // 
      }
      
    } else if (pwmValue == HIGH) {
    
      if (!is_ready) {
        // HIGHの途中から見てしまっている可能性があるので無視
        if (!log_ignore_high) {
          mhzlog(F("HIGH but !is_ready. ignore"));
          log_ignore_high = true;
        }
      } else if (WAIT_TGT == "HIGH") {
        // HIGH && is_ready && !is_wait_high = HIGH計測スタート
        WAIT_TGT = "LOW";
        high_start_ms = millis();
        mhzlog("HIGH measure HIGH start. waiting LOW ms=" + String(high_start_ms) );
      } else if (WAIT_TGT == "HIGH2") {
        // LOW->HIGH LOW計測終了。　これで計測完了
        long end_ms = millis();
        mhz_low_ms = end_ms - low_start_ms;
        mhzlog("HIGH measure LOW done. next: none. waiting none ms=" + String(end_ms) );     

        return;
      } else {
        // HIGH && is_ready && is_high = 計測中
        // 何もしない（まつだけ）
      }
    } else {
      mhzlog("not expected value " + String(pwmValue) );
    }
  } // while

  mhzlog(F("PWM Timeout"));
  mhz_high_ms = -1;
  mhz_low_ms = -1;
}

/**
 * 
 */
int mhz_read_data_pwm() {

  mhz_read_pwm_ms();

  if (mhz_high_ms < 0 || mhz_low_ms < 0) {
    mhzlog(F("PWM timeout, set lastppm = -999"));
    sensorValues.co2ppm = CO2_PPM_INVALID;
    return CO2_PPM_INVALID;
  }

  // total may 1004ms +- 5%
  mhzlog("mhz_high_ms = " + String(mhz_high_ms) + " mhz_low_ms  = " + String(mhz_low_ms) 
                 + " total = " + String(mhz_high_ms + mhz_low_ms));

  float ppm = 5000 * (mhz_high_ms - 2) / ((mhz_high_ms + mhz_low_ms) - 4);

  mhzlog("PPM = " + String(ppm,2) + " ppm");

  // PWMでよんだときに多少ブレてしまい、390台になるので、そこは400にごまかす
  if (385 < ppm && ppm < 399) {
    mhzlog(F("PPM is near 400. Result PPM adjusted to 400."));
    ppm = 400.0;
  }

  sensorValues.co2ppm = ppm;
  return (int) ppm;
}

void mhz_setup_pwm() {
  
  if (config.get(ConfigNames::MHZ19B) == ConfigValues::MHZ_NOUSE) {
    mhzlog(F("disabled."));
    return;
  }

  CO2_IN = config.get(ConfigNames::MHZ19B_PWM).toInt();
  pinMode(CO2_IN, INPUT);
  mhzlog("Enabled (PWM mode). GPIO pin = " + String(CO2_IN) );
}