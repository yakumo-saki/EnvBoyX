#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

// String _alerts_to_backup_text(config_alert_t& alerts, String keyPrefix) {
//   String ret = "";
//   ret +=       keyPrefix + "." + ConfigNames::ALERT_WARN1_LO +"=" + alerts.warning1.low;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN1_HI + "=" + alerts.warning1.high;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN2_LO + "=" + alerts.warning2.low;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN2_HI + "=" + alerts.warning2.high;

//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION1_LO + "=" + alerts.caution1.low;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION1_HI + "=" + alerts.caution1.high;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION2_LO + "=" + alerts.caution2.low;
//   ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION2_HI + "=" + alerts.caution2.high;
//   return ret;
// }

String http_api_backup_config() {
  String ret = "curl -X POST ";
  ret += "-d \"";
  
  for (auto &key : config.getKeys()) {
    ret += "&" + key + "=" + config.get(key);
  } 

  ret += "\"";
  ret += " http://" + config.get(ConfigNames::MDNS) + ".local/config";

  return ret;
}
