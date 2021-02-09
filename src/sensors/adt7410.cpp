#include <Arduino.h>
#include <Wire.h>

#include "global.h"
#include "log.h"

const uint8_t ADT7410_ADDR = 0x48;

bool use_adt7410 = false;

sensor_characters_t adt_characters() {
	sensor_characters_t sensor;

	sensor.temperature = true;
	return sensor;
}

bool has_adt7410()
{

	Wire.beginTransmission(ADT7410_ADDR); // S.C発行,CB送信
	byte error = Wire.endTransmission();

	//  if (error != 0) {
	//    Serial.print("Error adt7410 connection: " + error);
	//  }
	return (error == 0);
}

// 初期化
bool adt_setup(void)
{
	if (!has_adt7410())
	{
		adtlog(F("ADT7410 disabled"));
		return false;
	}

	Wire.beginTransmission(ADT7410_ADDR);
	Wire.write(0x03);
	Wire.write(0x00 | 0x80);
	Wire.endTransmission();
	use_adt7410 = true;
	return true;
}

// メインループ
void adt_read_data(void)
{

	if (!use_adt7410)
	{
		return;
	}

	uint16_t val0;
	float tmp0;
	long int ival0;

	Wire.requestFrom(ADT7410_ADDR, (uint8_t)2); // S.C発行,CB送信
	val0 = (uint16_t)Wire.read() << 8;			// データの読み出し(上位)
	val0 |= Wire.read();						// データの読み出し(下位)

	ival0 = (long int)val0;
	if (val0 & 0x8000)
	{ // 符号判定
		// 負数
		ival0 = ival0 - 65536;
	}
	tmp0 = (float)ival0 / 128.0;

	adtlog("Temp is " + String(tmp0));

	sensorValues.temperature = tmp0;
}
