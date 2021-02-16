#!/bin/bash -eu
#URL=http://ebx32.local/config
# using avahi causes script slow
URL=http://` avahi-resolve -n ebx32.local | awk '{print $2}'`/config
echo "URL = ${URL}"

echo "TEST: revert without commit"

echo "show"
curl -s "${URL}?key=displayBrightness" | jq
echo "set brightness 42"
curl -s -X POST -d "displayBrightness=42" "${URL}" | jq
echo "revert"
curl -s -X POST "${URL}/revert" | jq
echo "show"
curl -s "${URL}?key=displayBrightness" | jq

echo ""
read -p "Check displayFlip are reverted : "
clear

echo "TEST: revert and commit"
echo "set brightness 200"
curl -s -X POST -d "displayBrightness=200" "${URL}" | jq
echo "commit"
curl -s -X POST "${URL}/commit" | jq
echo "set brightness 255"
curl -s -X POST -d "displayBrightness=255" "${URL}" | jq
echo "revert to 200"
curl -s -X POST "${URL}/revert" | jq
echo "show"
curl -s "${URL}?key=displayBrightness" | jq

read -p "Check displayBrightness=200 hit enter: "
clear

echo "done"