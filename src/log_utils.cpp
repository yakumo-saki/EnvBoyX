#include <Arduino.h>

String get_i2c_error_message(byte code) {
    
    if (code == 0) {
        return F("code 0:success (not error)");
    } else if (code == 1) {
        return F("code 1:data too long to fit in transmit buffer");
    } else if (code == 2) {
        return F("code 2:received NACK on transmit of address");
    } else if (code == 3) {
        return F("code 3:received NACK on transmit of data");
    } else if (code == 4) {
        return F("code 4:other error");
    }
       
    return "code " + String(code) + ":unknown error";
}
