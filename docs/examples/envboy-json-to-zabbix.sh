#!/bin/bash

# This is example script 
# envboy-json-to-zabbix.sh ZBXHOSTNAME URL

set -u

MYDIR=$(cd "$(dirname "$0")"; pwd)
source ${MYDIR}/define.sh

HOST=$1
JSON=/tmp/envboy-${HOST}.json
URL=$2

rm -f ${JSON}
wget $WGET_OPT -O ${JSON} ${URL}

if [ -s ${JSON} ]; then
  echo "OK"
else
  ls -lh ${JSON}
  echo "JSON is broken. retry"
  sleep 3
  wget $WGET_OPT -O ${JSON} ${URL}
fi

if [ -s ${JSON} ]; then
  echo "maybe valid json. continue"
else
  echo "FILE SIZE = 0! JSON maybe broken. abort"
  exit 16
fi

PRODUCT=`jq '.product' ${JSON}`
UPTIME=`jq '.uptimeMills' ${JSON}`
TEMP=`jq '.temparature' ${JSON}`
HUMI=`jq '.humidity' ${JSON}`
PRES=`jq '.pressure' ${JSON}`
LUX=`jq '.luminous' ${JSON}`
IR=`jq '.luminousIr' ${JSON}`
CO2=`jq '.co2ppm' ${JSON}`
CO2_ACC=`jq '.co2ppmAccuracy' ${JSON}`

${ZBX_SENDER} -s ${HOST} -k envboy.product -o "${PRODUCT}"
${ZBX_SENDER} -s ${HOST} -k temperature -o "${TEMP}"
${ZBX_SENDER} -s ${HOST} -k humidity -o "${HUMI}"
${ZBX_SENDER} -s ${HOST} -k pressure -o "${PRES}"
${ZBX_SENDER} -s ${HOST} -k luminous -o "${LUX}"
${ZBX_SENDER} -s ${HOST} -k luminousIR -o "${IR}"
${ZBX_SENDER} -s ${HOST} -k uptimeMillis -o "${UPTIME}"


RSSI=`jq '.rssi' ${JSON}`
if [ "$RSSI" = "null" ]; then
  echo "no rssi values."
else
  ${ZBX_SENDER} -s ${HOST} -k rssi -o "${RSSI}"
fi

# co2 sensor often return ERROR
set +e
echo "CO2 is ${CO2}"

expr "$CO2 + 1" >&/dev/null
ret=$?

echo ret is $ret
if [ $ret -lt 2 ];then
  # input is number
  ${ZBX_SENDER} -s ${HOST} -k co2ppm -o "${CO2}"
  ${ZBX_SENDER} -s ${HOST} -k co2ppmAccuracy -o "${CO2_ACC}"
else
  echo "CO2 Sensor said ERROR. skip sending !"
fi
set -e

exit 0
