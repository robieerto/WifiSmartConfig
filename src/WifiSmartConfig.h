/*
 Library for configuration over Wifi with AP,
    mDNS, HTTP server and OTA updates
*/

#ifndef WIFI_SMART_CONFIG_H
#define WIFI_SMART_CONFIG_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <FS.h>

#define HOSTNAME "esp8266"
#define WEB_PORT 80
#define EEPROM_SIZE 512

struct credentials_t {
  char name[32];
  char pass[32];
};

extern ESP8266WiFiMulti wifiMulti;
extern ESP8266WebServer server;

credentials_t readCredentials();
void writeCredentials(credentials_t credentials);
void clearEEPROM();
void startWifi(WiFiMode_t wifiMode);
void startSoftAP(const char *ap_ssid, const char *ap_pass);
void startMDNS();
void startSPIFFS();
void startHTTP();
String handleClient();
void sendDataToWebView(String data);
void startOTA(const char *OTAName, const char *OTAPass);

// Helper functions
String formatBytes(size_t bytes);
void handleSave();
String getContentType(String filename);
bool handleFileRead(String path);

#endif