#ifndef defines_h
#define defines_h

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define ASYNC_MQTT_DEBUG_PORT               Serial
#define _ASYNC_MQTT_LOGLEVEL_               1

#define USE_ETHERNET_PORTENTA_H7            false
#define USE_WIFI_PORTENTA_H7                true

#include <WiFi.h>

#if (_ASYNC_MQTT_LOGLEVEL_ > 3)
  #warning Using WiFi for Portenta_H7
#endif

#define WIFI_SSID         "TP-LINK_87C91E"
#define WIFI_PASSWORD     ""

int status = WL_IDLE_STATUS;

#endif
