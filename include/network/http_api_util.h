#include <Arduino.h>

bool parseBooleanString(const String value);
std::vector<String> stringSplit(String value, String delimiter);
String jsonToString(DynamicJsonDocument& json);