#!/bin/bash -eu
#URL=http://ebx32.local/config
# using avahi causes script slow
URL=http://` avahi-resolve -n ebx32.local | awk '{print $2}'`/config
echo "URL = $URL"

echo "TEST: Single values at once"
echo "change DisplayFlip yes"
curl -s -X POST -d "displayFlip=yes" "$URL" | jq

echo ""
read -p "Check displayFlip on EnvBoy and hit enter: "

echo "TEST: Multiple values at once"
echo "change DisplayFlip no & brightness 50"
curl -s -X POST -d "displayFlip=no&displayBrightness=50" "$URL" | jq

read -p "Check displayFlip on EnvBoy and hit enter: "
clear

echo "TEST: Multiple values at once (Alerts)"
echo "change TempAlerts.warning2 10 ~ 40 and show it"
curl -s -X POST -d "tempAlerts.warning2.low=10&tempAlerts.warning2.high=40" "$URL" | jq

echo "show tempAlerts"
curl -s "$URL?key=tempAlerts" | jq

echo ""
read -p "Check tempAlerts warning2 and hit enter: "
clear

