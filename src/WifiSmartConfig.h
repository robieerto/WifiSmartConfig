/*
 Library for configuration over Wifi with AP,
    mDNS, HTTP server and OTA updates
*/

#ifndef WIFI_SMART_CONFIG_H
#define WIFI_SMART_CONFIG_H

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <FS.h>

#define AP_SSID "esp8266AP"
#define AP_PASS "heslojekreslo"
#define WEB_PORT 80

struct credentials_t {
  char name[32];
  char pass[32];
};

extern ESP8266WiFiMulti wifiMulti;
extern ESP8266WebServer server;

credentials_t readCredentials();
void writeCredentials(credentials_t credentials);
void startWifi(WiFiMode_t wifiMode);
void startSoftAP();
void startMDNS(const char *hostname);
void startSPIFFS();
void startHTTP();
void startOTA(const char *OTAName, const char *OTAPass);

// Helper functions
String formatBytes(size_t bytes);
void handleSave();
String getContentType(String filename);
bool handleFileRead(String path);

#endif