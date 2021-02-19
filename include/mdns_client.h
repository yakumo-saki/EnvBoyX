#include <Arduino.h>

// mdns.h ではないのは、 dunk mdnsのヘッダファイルが mdns.h なため

void mdns_setup();
void mdns_loop();

void mdns_hostname_change(String hostname);
