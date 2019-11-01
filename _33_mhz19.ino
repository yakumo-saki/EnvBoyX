#include <MHZ19.h>
#include <SoftwareSerial.h> 

// MHZ用software serial のピン
const int MHZ_RX_PIN = 12;
const int MHZ_TX_PIN = 14;

// wait for co2 sensor warmup (maybe forever).
bool WAIT_FOR_CO2_WARMUP = false;
bool WAIT_FOR_CO2_WARMUP_FOREVER = false;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;


MHZ19 mhz;                                             // Constructor for MH-Z19 class
SoftwareSerial mhzSerial(MHZ_RX_PIN, MHZ_TX_PIN);                   // Uno example

unsigned long mhzGetDataTimer = 0;                     

extern int lastPpm;

void mhzlog(String msg) {
  Serial.println("MHZ19B: " + msg);
}

void mhz_setup() {

  if (use_mhz19b == USE_NO) {
    mhzlog("disabled.");
    return;
  } else {
    mhzlog(use_mhz19b);
    mhzlog(USE_NO);
  }

  mhzlog("initializing.");
  
  mhzSerial.begin(9600);
  mhz.begin(mhzSerial);
  mhz.autoCalibration(false);
  mhzlog("Start software serial.");
}

void dummy() {
//  while( mhz19->isWarming() ) {
//  Serial.print(wait);
//  Serial.print(" MH-Z19 now warming up...  status:");
//  Serial.println(mhz19->getStatus());
//  delay(1000);
//  wait++;
//  if (wait > 3) {
//    Serial.println("warmup timeout continue anyway");      
//    break;
//  }
}

void mhz_read_data() {

  if (use_mhz19b == USE_NO) {
    return;
  }

  if (millis() - mhzGetDataTimer >= 2000) {
    int CO2;                                            // Buffer for CO2
    CO2 = mhz.getCO2();                             // Request CO2 (as ppm)

    mhzlog("CO2 (ppm): " + String(CO2));                      

    int8_t Temp;                                         // Buffer for temperature
    Temp = mhz.getTemperature();                     // Request Temperature (as Celsius)
    mhzlog("Temperature (C): " + String(Temp));         

    mhzGetDataTimer = millis();                            // Update interval
  }
}
