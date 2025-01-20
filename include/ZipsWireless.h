#ifndef USING_PASSWORD
#define USING_PASSWORD 0
#endif

#define WIFI_SSID "juicy_ass"
#if USING_PASSWORD
#define WIFI_PASSWORD "zane"
#else
#define WIFI_PASSWORD 0
#endif
#define TIMEOUT_MS 20000

#ifndef RETRY_CONNECTION
#define RETRY_CONNECTION 1
#endif