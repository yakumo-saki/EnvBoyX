/**
 * WiFi接続をする。
 * @param haltOnFail 失敗した場合停止し、setup modeに落ちる
 */
void make_sure_wifi_connected(bool haltOnFail);

void start_wifi_access_point();

String get_wifi_ip_addr();

void wifi_store_rssi();
