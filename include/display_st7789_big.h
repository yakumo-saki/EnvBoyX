#include <Arduino.h>
#include "structs.h"

void _clear_screen_big();
void _disp_header_big(String ip, String mDNS);
void _disp_sensor_value_big(disp_values_t val);