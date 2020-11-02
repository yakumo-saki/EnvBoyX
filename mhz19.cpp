#include <SoftwareSerial.h>
#include <MHZ19.h>

#include "define.h"
#include "global_extern.h"

// ■ 動作モード ■
bool USE_PWM = true;

// MHZ用software serial のピン
const int MHZ_RX_PIN = 9;
const int MHZ_TX_PIN = 10;

const int CO2_PPM_INVALID = -999;

// wait for co2 sensor warmup (maybe forever).
bool WAIT_FOR_CO2_WARMUP = false;
bool WAIT_FOR_CO2_WARMUP_FOREVER = false;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;

// PWM
int CO2_IN = -1;

// 0 ~ 1004までなのでintでOK
int mhz_high_ms = 0;
int mhz_low_ms = 0;

MHZ19 mhz19;
SoftwareSerial mhzSerial(MHZ_RX_PIN, MHZ_TX_PIN);

unsigned long mhzGetDataTimer = 0;                     

void setRange(int range);
void printErrorCode();

void mhzlog(String msg) {
  Serial.println("MHZ19B: " + msg);
}

void mhz_setup() {
  if (use_mhz19b == MHZ_NOUSE) {
    mhzlog("disabled.");
    return;
  } 
  
  USE_PWM = (use_mhz19b == MHZ_USE_PWM);

  if (USE_PWM) {
    mhz_setup_pwm();
  } else {
    mhz_setup_uart();
  }
}

void mhz_setup_pwm() {
  
  if (use_mhz19b == MHZ_NOUSE) {
    mhzlog("disabled.");
    return;
  }

  CO2_IN = mhz19b_pwmpin.toInt();

  mhzlog("Enabled (PWM mode). GPIO pin = " + String(CO2_IN) );
}

void mhz_setup_uart() {

  lastPpm = CO2_PPM_INVALID;

  mhzlog("Enabled (UART mode).");

  mhzSerial.begin(MHZ_BAUDRATE);
  mhz19.begin(mhzSerial);
  setRange(2000);

  // mhz19.setAutoCalibration(AUTO_BASELINE_CORRECTION);
  // if (AUTO_BASELINE_CORRECTION) {
  //   mhzlog("WARNING -------------------------- WARNING");
  //   mhzlog("     Auto Baseline Correction is ON!");
  //   mhzlog("WARNING -------------------------- WARNING");
  // }

  // see https://platformio.org/lib/show/6091/MH-Z19
  if (mhz19.errorCode == RESULT_OK)
        mhz19.calibrateZero();                            // Calibrate
    else
      printErrorCode();

  if (mhz19.errorCode == RESULT_OK)
      mhz19.setSpan(2000);                               // Set Span 2000
  else
      printErrorCode();

  if (mhz19.errorCode == RESULT_OK)
      mhz19.autoCalibration(false);                       // Turn auto calibration OFF
  else
      printErrorCode();

  mhzlog("initialized.");
 
}

void mhz_read_data() {

  if (use_mhz19b == MHZ_NOUSE) {
    return;
  }

  if (USE_PWM) {
    mhz_read_data_pwm();
  } else {
    mhz_read_data_uart();
  }

}

void mhz_read_data_uart() {

  if ( (millis() - mhzGetDataTimer) < 3000) {
    return;
  }

  mhzGetDataTimer = millis();
  int co2ppm = mhz19.getCO2();
  int temp = mhz19.getTemperature();
  
  mhzlog("CO2 (ppm): " + String(co2ppm) + " Temp: " + String(temp) );
  lastPpm = co2ppm;

}

String mhz19_code_to_msg(int error_code) {

  if (error_code == 0) {
    return "RESULT IS NULL";
  } else if (error_code == 1) {
    return "RESULT_OK";
  } else if (error_code == 2) {
    return "RESULT_TIMEOUT";
  } else if (error_code == 3) {
    return "RESULT_MATCH";
  } else if (error_code == 4) {
    return "RESULT_CRC";
  } else if (error_code == 5) {
    return "RESULT_FILTER";
  } else if (error_code == 6) {
    return "RESULT_FAILED";
  }

  return "UNKNOWN CODE " + String(error_code); 
}


String mhz_code_to_msg(int error_code) {

  if (error_code == -2) {
    return "NO_RESPONSE";
  } else if (error_code == -3) {
    return "STATUS_CHECKSUM_MISMATCH";
  } else if (error_code == -4) {
    return "STATUS_INCOMPLETE";
  } else if (error_code == -5) {
    return "STATUS_NOT_READY";
  } else if (error_code == -6) {
    return "STATUS_PWM_NOT_CONFIGURED";
  } else if (error_code == -7) {
    return "STATUS_SERIAL_NOT_CONFIGURED";
  }

  return "UNKNOWN CODE " + String(error_code); 
}

/**
 * 
 */
int mhz_read_data_pwm() {

  mhz_read_pwm_ms();

  if (mhz_high_ms < 0 || mhz_low_ms < 0) {
    mhzlog("PWM timeout, set lastppm = -999");
    lastPpm = CO2_PPM_INVALID;
    return CO2_PPM_INVALID;
  }

  // total may 1004ms +- 5%
  mhzlog("mhz_high_ms = " + String(mhz_high_ms) + " mhz_low_ms  = " + String(mhz_low_ms) 
                 + " total = " + String(mhz_high_ms + mhz_low_ms));

  float ppm = 5000 * (mhz_high_ms - 2) / ((mhz_high_ms + mhz_low_ms) - 4);

  mhzlog("PPM = " + String(ppm,2) + " ppm");

  // PWMでよんだときに多少ブレてしまい、390台になるので、そこは400にごまかす
  if (385 < ppm && ppm < 399) {
    mhzlog("PPM is near 400. Result PPM adjusted to 400.");
    ppm = 400.0;
  }

  lastPpm = ppm;
  return (int) ppm;
}

/**
 * worst expected execution time 2008ms + alpha
 */
void mhz_read_pwm_ms() {

  const int TIMEOUT_MS = 2500;
  
  long high_start_ms = 0;
  long low_start_ms = 0;

  bool is_ready = false;

  String WAIT_TGT = "";

//  bool log_wait_for_low  = false;
//  bool log_wait_for_high = false;
  bool log_ignore_high = false;

  long timeout_start_ms = millis();
  long timeout_end_ms = timeout_start_ms + TIMEOUT_MS;
  mhzlog("read_mhz_pwm start " + String(timeout_start_ms));

  while (millis() < timeout_end_ms) {
    int pwmValue = digitalRead(CO2_IN);
    ESP.wdtFeed();

    // Serial.println(pwmValue);
    
    if (pwmValue == LOW) {
      if (!is_ready) {
        // LOWになったので次のHIGHから計測開始
        is_ready = true;
        WAIT_TGT = "HIGH";
        mhzlog("LOW wait for first HIGH ms=" + String(millis()) );
      } else if (WAIT_TGT == "LOW") {
        // 計測開始後、HIGH→LOW になった時(HIGH計測終了）。次はLOW計測
        long end_ms = millis();
        low_start_ms = end_ms;
        WAIT_TGT = "HIGH2";
        long elapsed = end_ms - high_start_ms;
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
          mhzlog("HIGH but !is_ready. ignore");
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

  mhzlog("PWM Timeout");
  mhz_high_ms = -1;
  mhz_low_ms = -1;
}
