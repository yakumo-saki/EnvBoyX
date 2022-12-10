#include <Arduino.h>
#include <vector>

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

// returns string exist in vector<String>
bool vectorStringContains(const std::vector<String> &keyArray, const String& key) {

  for (const String k : keyArray) {
    if (k == key) { 
      // debuglog("return true " + key);
      return true;
    }
  }
  // debuglog("return false " + key);
  return false;
}
