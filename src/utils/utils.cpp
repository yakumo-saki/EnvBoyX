#include <Arduino.h>
#include <vector>

#include "log.h"

/**
 * 文字列をboolとして解釈する。大文字小文字は区別しない
 * TRUE/1/ON/YES
 * @return bool TRUE-ish ならTRUEそれ以外ならfalse
*/
bool parseBooleanString(const String val) {

    // String is null or invalid
    if (!val) return false;

    String v = String(val);
    v.toUpperCase();

    if (v == "TRUE" ) return true;
    if (v == "1") return true;
    if (v == "ON") return true;
    if (v == "YES") return true;

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
