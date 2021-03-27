#!/bin/bash

URL=http://` avahi-resolve -n ebx32.local | awk '{print $2}'`/config
echo "URL = $URL"

echo "TEST: Get Single"
curl "${URL}?key=displayBrightness" | jq

read -p "Check and hit enter: "
clear

echo "TEST: Get Single (Alerts)"
curl "${URL}?key=humiAlerts" | jq

read -p "Check and hit enter: "
clear

echo "TEST: Get Multi (ALL)"
curl "${URL}?key=humiAlerts,co2Alerts,humiAlerts,luxAlerts,presAlerts,mqttName,mqttBroker,mhz19bTxPin,mhz19bRxPin,mhz19bPwmPin,mhz19b,st7789Mode,st7789,oledType,displayBrightness,displayFlip,opMode,mDNS,password,ssid" | jq

read -p "Check and hit enter: "
clear

echo "done"