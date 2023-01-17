#include "FS.h"

// 

/** 
 * SPIFFS/LittleFS.open を実行する
 * @return fs::File ファイルクラス
*/
File fsOpen(String path, char *mode);

/** 
 * SPIFFS/LittleFS.remove を実行する
*/
bool fsRemove(String path);


#ifdef ESP32
#define EBXFS SPIFFS
#endif

#ifdef ESP8266
#define EBXFS LITTLEFS
#endif