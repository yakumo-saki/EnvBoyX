#include <Arduino.h>

#include "log.h"

bool parseBooleanString(const String val) {

    // String is null or invalid
    if (!val) return false;

    String v = String(val);
    v.toUpperCase();

    if (v == "TRUE" ) return true;
    if (v == "1") return true;
    if (v == "ON") return true;
    if (v == "YES") return true;

    if (v == "FALSE" ) return false;
    if (v == "0") return false;
    if (v == "OFF") return false;
    if (v == "NO") return false;

    return false;
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
            ret.push_back(temp);
            temp = "";
            continue;
        } else {
            temp += v;
        }
    }
    
    ret.push_back(temp);

    return ret;
}