/*
 Smart configuration over Wifi with AP,
  mDNS, HTTP server and OTA updates
*/

#include "WifiSmartConfig.h"

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(WEB_PORT);

credentials_t readCredentials() {
  uint16_t addr = 0;
  credentials_t credentials;
  EEPROM.begin(512);
  EEPROM.get(addr, credentials);
  EEPROM.end();
  return credentials;
}

void writeCredentials(credentials_t credentials) {
  uint16_t addr = 0;
  EEPROM.begin(512);
  EEPROM.put(addr, credentials);
  EEPROM.commit();
  EEPROM.end();
}

void clearEEPROM() {
  EEPROM.begin(512);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();
}

// Connect to one of the saved Wifi SSID's
void startWifi(WiFiMode_t wifiMode) {
  // Setup Wifi, WIFI_STA - only station,
  //  WIFI_AP - only hotspot, WIFI_AP_STA - both
  WiFi.mode(wifiMode);
  if (wifiMode == WIFI_STA || wifiMode == WIFI_AP_STA) {
    Serial.print("Connecting ...");
    while (wifiMulti.run() != WL_CONNECTED) {
      delay(250);
      Serial.print('.');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
  }
}

// Setup soft AP
void startSoftAP() {
  Serial.print("Setting soft-AP ... ");
  IPAddress apLocalIp(192, 168, 4, 1);
  IPAddress apGatewayIp(192, 168, 4, 1);
  IPAddress apSubnetMask(255, 255, 255, 0);
  WiFi.softAPConfig(apLocalIp, apLocalIp, apSubnetMask);
  credentials_t credentials = readCredentials();
  if (! WiFi.softAP(credentials.name, credentials.pass)) {
    Serial.print(WiFi.softAP(AP_SSID, "")
      ? "Ready" : "Failed!");
    Serial.println(" as " + String(AP_SSID));
  }
  else {
    Serial.println("Ready as " + String(credentials.name));
  }
}

// Start the mDNS responder for specific hostname
void startMDNS(const char *hostname) {
  if (MDNS.begin(hostname)) {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

// Start the SPI Flash Files System
void startSPIFFS() {
  SPIFFS.begin();                             
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

// Start HTTP server
void startHTTP() {
  // Handle HTTP requests
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

// Start over-the-air update service
void startOTA(const char *OTAName, const char *OTAPass) {
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPass);
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

// Convert sizes in bytes to KB and MB
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

// Handle request for saving credentials
void handleSave() {
  if (!server.hasArg("name") || !server.hasArg("password") 
      || server.arg("name") == NULL || server.arg("password") == NULL) {
    handleFileRead("/incorrect.html");
    return;
  }
  credentials_t credentials;
  strcpy(credentials.name, server.arg("name").c_str());
  strcpy(credentials.pass, server.arg("password").c_str());
  writeCredentials(credentials);
  handleFileRead("/success.html");
  delay(3000);
  ESP.restart();
}

// Convert the file extension to the MIME type
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

// Send the right file to the client (if it exists)
bool handleFileRead(String path) {
  path = "/web" + path;
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.html";           // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){  // If the file exists, either as a compressed archive, or normal
    if(SPIFFS.exists(pathWithGz))                          // If there's a compressed version available
      path += ".gz";                                       // Use the compressed version
    File file = SPIFFS.open(path, "r");                    // Open the file
    server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;                                          // If the file doesn't exist, return false
}