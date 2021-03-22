#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"

String jsonToString(DynamicJsonDocument& json) {
    String jsonStr;
    serializeJson(json, jsonStr);
    return jsonStr;
}

std::vector<String> stringSplit(String value, String delimiter) {
    std::vector<String> ret;

    String temp = "";
    for (unsigned int i = 0; i < value.length(); i++) {
        String v;
        if (i + 1 > value.length()) {
            v = value.substring(i);
        } else {
            v = value.substring(i, i + 1);
        }
        if (v == delimiter) {
            temp.trim();
            ret.push_back(temp);
            temp = "";
        } else {
            temp += v;
        }
    }
    
    temp.trim();
    ret.push_back(temp);

    return ret;
}