#include <Arduino.h>
#include "structs.h"

void _clear_screen_normal();
void _disp_header_normal(String ip, String mDNS);
void _disp_sensor_value_normal(disp_values_t val);