#define SETTING_ID "EBX.r4"

#define OPMODE_MQTT "mqtt"
#define OPMODE_DISPLAY "always"
#define MHZ_USE_PWM "yes_pwm"
#define MHZ_USE_UART "yes_uart"
#define MHZ_NOUSE "no"

// Wi-Fi設定保存ファイル
String settings = "/wifi_settings.txt";
String configured_file = "/config_ok.txt";

String product_short = "ebx";
String product = "EnvBoyX";
String ver = "2.0";
String product_long = product + " Ver." + ver;
