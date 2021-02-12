#include <Arduino.h>

#include <Wire.h>
#include <Tsl2561.h>

#include "global.h"
#include "log.h"

Tsl2561 Tsl(Wire);

bool use_tsl = false;

sensor_characters_t tsl_characters() {
	sensor_characters_t sensor;

	sensor.lux = true;
	sensor.luxIr = true;

	return sensor;
}


bool tsl_setup()
{
	// init TSL2561
	Tsl.begin();
	if (!Tsl.available())
	{
		tsllog(F("TSL2561 disabled."));
		return false;
	}
	else
	{
		tsllog(F("TSL2561 Enabled"));
		use_tsl = true;
		return true;
	}
}

//
void read_data_tsl2561()
{
	uint8_t id;
	uint16_t full, ir;

	if (!use_tsl || !Tsl.available())
	{
		return;
	}

	Tsl.on();

	Tsl.setSensitivity(false, Tsl2561::EXP_402);
	//delay(16);
	//delay(110);
	delay(402 + 10);

	Tsl.id(id);
	Tsl.fullLuminosity(full);
	Tsl.irLuminosity(ir);

	char log[80];
	snprintf(log, sizeof log, "Luminosity: %5u lx IR: %5u lx", full, ir);
	tsllog(String(log));

	sensorValues.lux = full;
	sensorValues.luxIr = ir;

	Tsl.off();
}
