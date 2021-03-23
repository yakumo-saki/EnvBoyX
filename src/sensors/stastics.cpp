#include <Arduino.h>
#include <ArduinoJson.h>

#include "global.h"
#include "log.h"
#include "TimerCall.h"

// 統計情報を取得
void updateStastics(std::vector<TimerCall::TimerCallTask> &tasks) {
  const String STAT = "stastics";

  DynamicJsonDocument doc(500);
  doc["time"] = millis();

  int idx = 0;
  for (auto it = tasks.begin(), e = tasks.end(); it != e; ++it) {
    // statlog(+"name=" + String(it->info.name) +
    //         " last=" + String(it->info.lastExecMills) +
    //         " last exec=" + String(it->info.lastElapsedMills) +
    //         " total=" + String(it->info.totalElapsedMills) +
    //         " count=" + String(it->info.callCount));

    // 統計
    doc[STAT][idx]["name"] = String(it->info.name);
    doc[STAT][idx]["lastExecMs"] = it->info.lastExecMills;
    doc[STAT][idx]["lastElapsedMs"] = it->info.lastElapsedMills;
    doc[STAT][idx]["totalElapsedMs"] = it->info.totalElapsedMills;
    doc[STAT][idx]["callCount"] = it->info.callCount;
    idx++;
  }

  String logmsg = "";
  if (DEBUG_BUILD) logmsg += "**DEBUG BUILD** ";

  logmsg += "Statstics updated.";
#ifdef ESP32
  doc["cputemp"] = temperatureRead();  // CPU温度
  logmsg += " cpuTemp=" + String(temperatureRead());
  logmsg += " freeHeap=" + String(ESP.getFreeHeap());
#endif

#ifdef ESP8266
  logmsg += " freeHeap=" + String(ESP.getFreeHeap());
#endif

  statlog(logmsg); // これくらいは出しておかないと動いてるのかわからなくなるので出す

  String json = "";
  serializeJson(doc, json);
  stasticsJSON = json;
}