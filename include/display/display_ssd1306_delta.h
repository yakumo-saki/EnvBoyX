#include <Arduino.h>

enum class DeltaType {
  None = 0, Temp = 1, Humi = 2, Pressure = 3, co2 = 4
};

typedef struct {
    DeltaType deltaType1;
    DeltaType deltaType2;
} delta_types_t;

// 表示するべきDeltaの種別を返す。
DeltaType get_delta_type();

// 表示するべきDeltaの種別を2個返す。
delta_types_t get_delta_type2();