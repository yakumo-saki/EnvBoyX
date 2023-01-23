#include <Arduino.h>

#include <LittleFS.h>

#include "log.h"
#include "global.h"
#include "config/config.h"

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  cfglog(F(">>> LittleFS directory listing"));
 
  File root = LittleFS.open("/", "w");
  if (!root) {
      cfglog(F("- failed to open directory"));
      return;
  }
  if (!root.isDirectory()){
      cfglog(F(" - not a directory"));
      return;
  }

  File file = root.openNextFile();
  while(file){
      if(file.isDirectory()){
          cfglog("  DIR : " + String(file.name()));
      } else {
          cfglog("  FILE: " + String(file.name()) + "  SIZE: " + String(file.size()));
      }
      file = root.openNextFile();
  }

  cfglog(F("<<< End listing"));
}
