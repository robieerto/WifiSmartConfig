/*
 Library for configuration over Wifi with AP,
    mDNS, HTTP server and OTA updates
*/

#ifndef WIFI_SMART_CONFIG_H
#define WIFI_SMART_CONFIG_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define HOSTNAME "esp8266"
#define EEPROM_SIZE 512

extern ESP8266WiFiMulti wifiMulti;
extern ESP8266WebServer server;

#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

#define HOSTNAME "esp32"
#define EEPROM_SIZE 1000

extern WiFiMulti wifiMulti;
extern WebServer server;
#endif

#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <FS.h>

#define WEB_PORT 80

struct credentials_t {
  char name[32];
  char pass[32];
  char flag[5];
};

struct ipAddresses_t {
  IPAddress apLocalIp;
  IPAddress apGateway;
  IPAddress apSubnetMask;
};

credentials_t readCredentials();
void writeCredentials(credentials_t credentials);
void clearCredentials();
void clearEEPROM();
void startWifi(WiFiMode_t wifiMode);
void startSoftAP(const char *ap_ssid, const char *ap_pass, ipAddresses_t ip_addresses);
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