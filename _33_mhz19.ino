// https://github.com/tobiasschuerg/MH-Z-CO2-Sensors
#include <MHZ19.h>
#include <SoftwareSerial.h> 

// MHZ用software serial のピン
const int MHZ_RX_PIN = 12;
const int MHZ_TX_PIN = 14;

const int CO2_PPM_INVALID = -999;

// wait for co2 sensor warmup (maybe forever).
bool WAIT_FOR_CO2_WARMUP = false;
bool WAIT_FOR_CO2_WARMUP_FOREVER = false;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = true;


MHZ19 mhz;                                             // Constructor for MH-Z19 class
SoftwareSerial mhzSerial(MHZ_RX_PIN, MHZ_TX_PIN);                   // Uno example

unsigned long mhzGetDataTimer = 0;                     

extern int lastPpm;

void mhzlog(String msg) {
  Serial.println("MHZ19B: " + msg);
}

void mhz_setup() {

  lastPpm = CO2_PPM_INVALID;

  if (use_mhz19b == USE_NO) {
    mhzlog("disabled.");
    return;
  }

  mhzlog("Enabled.");
  
  mhzlog("Start software serial.");
  mhzSerial.begin(9600);

  mhzlog("initializing.");
  mhz.begin(mhzSerial);
  mhz.autoCalibration(AUTO_BASELINE_CORRECTION);
  mhz.setRange(5000);
  mhzlog("initialized.");

  if (AUTO_BASELINE_CORRECTION) {
    mhzlog("WARNING -------------------------- WARNING");
    mhzlog("     Auto Baseline Correction is ON!");
    mhzlog("WARNING -------------------------- WARNING");
  }
  mhzlog("initialized.");
  
  
}

void mhz_read_data() {

  if (use_mhz19b == USE_NO) {
    return;
  }

  if ( (millis() - mhzGetDataTimer) >= 3000) {

    int MAX_RETRY = 3;

    int temp = 0;
    
    int retry_count = 0;
    while (retry_count <= MAX_RETRY) {
      temp = mhz.getTemperature();
      if (mhz.errorCode == 1) {
        break;
      }
      retry_count = retry_count + 1;
      delay(100);
    }

    retry_count = 0;
    while (retry_count <= MAX_RETRY) {
      int CO2 = mhz.getCO2();

      if (mhz.errorCode == 1) {
        mhzlog("CO2 (ppm): " + String(CO2) + + " Temp(C): " + String(temp)  + " Status: " + mhz_code_to_msg(mhz.errorCode));
        lastPpm = CO2;
        break;
      } else {
        mhzlog("Retry cause status: " + mhz_code_to_msg(mhz.errorCode));
      }
      retry_count = retry_count + 1;
      delay(500);
      if (retry_count == MAX_RETRY) {
        mhzlog("too many failure. exec setup again !");
        mhz_setup();
        lastPpm = -999;
      }
    }

    mhzGetDataTimer = millis();
  }
}

String mhz_code_to_msg(int error_code) {

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
