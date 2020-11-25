#include <Arduino.h>
#include "sensors/mhz19_main.h"

String mhz19_code_to_msg(byte error_code) {

  if (error_code == 0) {
    return "RESULT IS NULL";
  } else if (error_code == 1) {
    return "RESULT_OK";
  } else if (error_code == 2) {
    return "RESULT_TIMEOUT";
  } else if (error_code == 3) {
    return "RESULT_MATCH";
  } else if (error_code == 4) {
    return "RESULT_CRC";
  } else if (error_code == 5) {
    return "RESULT_FILTER";
  } else if (error_code == 6) {
    return "RESULT_FAILED";
  }

  return "UNKNOWN CODE " + String(error_code); 
}


String mhz_code_to_msg(int error_code) {

  if (error_code == -2) {
    return "NO_RESPONSE";
  } else if (error_code == -3) {
    return "STATUS_CHECKSUM_MISMATCH";
  } else if (error_code == -4) {
    return "STATUS_INCOMPLETE";
  } else if (error_code == -5) {
    return "STATUS_NOT_READY";
  } else if (error_code == -6) {
    return "STATUS_PWM_NOT_CONFIGURED";
  } else if (error_code == -7) {
    return "STATUS_SERIAL_NOT_CONFIGURED";
  }

  return "UNKNOWN CODE " + String(error_code); 
}

