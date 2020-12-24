#include <Arduino.h>

typedef struct {
    String ip;
    String mDNS;
    String temperature;
    String humidity;
    String pressure;
    String lux;
    String luxIr;
    String co2ppm;
} disp_values_t;
