#include <Arduino.h>

#include <Wire.h>

const uint8_t AM2320_ADDR = 0x5C;

extern float lastTemp;
extern float lastHumidity;

bool use_am2320 = false;

void amlog(String msg) {
  Serial.println("AM2320: " + msg);
}

void am_setup(void) {
  // wake from sleep
  Wire.beginTransmission(AM2320_ADDR);
  Wire.endTransmission();

  delay(5);
  
  Wire.beginTransmission(AM2320_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    amlog("Device not found. disabled code: " + String(error));
    return;
  }

  // 湿度・温度データ取得要求（読み捨て）
  Wire.beginTransmission(AM2320_ADDR);
  delay(1); // wakeup まち (0.8ms) 
  Wire.write(0x03);         // レジスタ読み取りコマンド
  Wire.write(0x00);         // 読み取り開始レジスタ番号
  Wire.write(0x04);         // 読み取りデータ数    
  Wire.endTransmission();

  use_am2320 = true;
}

void am_read_data() {

  if (!use_am2320) {
    return;
  }
  
  uint8_t data[8];

  // AM2320センサーのウェイクアップ
  Wire.beginTransmission(AM2320_ADDR);
  Wire.endTransmission();

  delay(1); // wakeup まち (0.8ms) 

  Wire.beginTransmission(AM2320_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    amlog("Error AM2320 connection: " + String(error));
    return;
  }

  // 湿度・温度データ取得要求（読み捨て）
  Wire.beginTransmission(AM2320_ADDR);
  delay(1); // wakeup まち (0.8ms) 
  Wire.write(0x03);         // レジスタ読み取りコマンド
  Wire.write(0x00);         // 読み取り開始レジスタ番号
  Wire.write(0x04);         // 読み取りデータ数    
  Wire.endTransmission();

  delay(2000);

  // 湿度・温度データ取得要求
  Wire.beginTransmission(AM2320_ADDR);
  delay(1); // wakeup まち (0.8ms) 
  Wire.write(0x03);         // レジスタ読み取りコマンド
  Wire.write(0x00);         // 読み取り開始レジスタ番号
  Wire.write(0x04);         // 読み取りデータ数    
  Wire.endTransmission();

  // データの取得
  delayMicroseconds(1500);
  Wire.requestFrom(AM2320_ADDR, (uint8_t) 8); 
  delayMicroseconds(50);
  
  if (Wire.available() >= 8) {
    for (uint8_t i = 0; i < 8; i++) {
      data[i] = Wire.read();
    }
    
    float rh = ((float)(data[2] * 256 + data[3])) / 10; // 湿度
    float tp = ((float)(data[4] * 256 + data[5])) / 10; // 温度
    
    // 湿度・温度の表示
    Serial.print("AM2320: ");    
    Serial.print("T=");
    Serial.print(tp);
    Serial.print("c");
    Serial.print(" H=");
    Serial.print(rh);
    Serial.println("%");

    lastTemp = tp;
    lastHumidity = rh;
  }
}