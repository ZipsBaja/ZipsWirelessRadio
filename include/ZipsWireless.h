#pragma once

#ifndef USING_PASSWORD
#define USING_PASSWORD 0
#endif

#define WIFI_SSID "ZipsBajaWirelessRadio"
#if USING_PASSWORD
#define WIFI_PASSWORD "uakron2025_jackie"
#else
#define WIFI_PASSWORD 0
#endif
#define WIFI_TIMEOUT_MS 20000
#define WIFI_PORT 3001

#define WIFI_PAYLOAD_HIGH 100
#define WIFI_PAYLOAD_LOW 0

#ifndef RETRY_CONNECTION
#define RETRY_CONNECTION 1
#endif