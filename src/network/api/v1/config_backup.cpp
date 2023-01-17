#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config/config.h"

String http_api_backup_config() {
  String ret = "curl -X POST ";
  ret += "-d ";
  ret += "\"";
  
  String keys = "";
  for (auto &key : config->getKeys()) {
    if (key == ConfigNames::PASSWORD) continue;
    if (key == ConfigNames::SETTING_ID) continue;

    keys += "&" + key + "=" + config->get(key);
  } 
  keys = keys.substring(1);

  ret += keys;

  ret += "\"";
  ret += " http://" + config->get(ConfigNames::MDNS) + ".local/api/v1/config";

  return ret;
}
