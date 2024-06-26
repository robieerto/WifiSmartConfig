/*
 Smart configuration over Wifi with AP,
  mDNS, HTTP server and OTA updates
*/

#include "WifiSmartConfig.h"
#include "pages.h"

#if defined(ESP8266)
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(WEB_PORT);
#elif defined(ESP32)
WiFiMulti wifiMulti;
WebServer server(WEB_PORT);
#endif

// EEPROM starting address
uint16_t startAddrCredentials = 0;
// Buffer for recieved data
String data_buffer = "";

// Read credentials from EEPROM
credentials_t readCredentials()
{
  credentials_t credentials;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(startAddrCredentials, credentials);
  EEPROM.end();
  return credentials;
}

// Write credentials to EEPROM
void writeCredentials(credentials_t credentials)
{
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(startAddrCredentials, credentials);
  EEPROM.commit();
  EEPROM.end();
}

bool areCredentialsSaved(credentials_t credentials)
{
  return strcmp(credentials.flag, "some") == 0;
}

// Clear credentials from EEPROM
void clearCredentials()
{
  credentials_t credentials = {0};
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(startAddrCredentials, credentials);
  EEPROM.commit();
  EEPROM.end();
}

// Clear the whole EEPROM
void clearEEPROM()
{
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.end();
}

// Connect to one of the saved Wifi SSID's
void startWifi(WiFiMode_t wifiMode)
{
  // Setup Wifi, WIFI_STA - only station,
  //  WIFI_AP - only hotspot, WIFI_AP_STA - both
  WiFi.mode(wifiMode);
  if (wifiMode == WIFI_STA || wifiMode == WIFI_AP_STA)
  {
    Serial.print("Connecting ...");
    while (wifiMulti.run() != WL_CONNECTED)
    {
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
void startSoftAP(const char *ap_ssid, const char *ap_pass, ipAddresses_t ip_addresses)
{
  Serial.println("Setting soft-AP ... ");
  WiFi.softAPConfig(ip_addresses.apLocalIp, ip_addresses.apGateway, ip_addresses.apSubnetMask);
  if (WiFi.softAP(ap_ssid, ap_pass))
  {
    Serial.print("Ready as " + String(ap_ssid));
    Serial.println(" with pass " + String(ap_pass));
  }
  else
  {
    Serial.print("AP Failed!");
  }
}

// Start the mDNS responder for specific hostname
void startMDNS()
{
  if (MDNS.begin(HOSTNAME))
  {
    Serial.println("mDNS responder started");
    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
  }
  else
  {
    Serial.println("Error setting up MDNS responder!");
  }
}

// Start the SPI Flash Files System
void startSPIFFS()
{
  SPIFFS.begin();
  Serial.println("SPIFFS started. Contents:");
  {
#if defined(ESP8266)
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
#elif defined(ESP32)
    File root = SPIFFS.open("/");
    File file;
    while (file = root.openNextFile())
    {
      String fileName = file.name();
      size_t fileSize = file.size();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
#endif
    Serial.printf("\n");
  }
}

// Start HTTP server
void startHTTP()
{
  // Handle HTTP requests
  server.on("/", HTTP_GET, []()
            {
    Serial.println("Root request: /");
    server.send_P(200, "text/html", MAIN_page); });

  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound([]()
                    {
    String uri = server.uri();
    if (uri.indexOf('*') == -1) {
      Serial.println("Request: " + uri);
      if (!handleFileRead(uri))
        server.send_P(404, "text/html", NOTFOUND_page);
    }
    else { // data from client application
    Serial.println("Request from app: " + uri);
      uri.remove(0, 1);
      data_buffer = uri;
    } });

  server.begin();
  Serial.println("HTTP server started");
}

// Handle incoming request from client
String handleClient()
{
#if defined(ESP8266)
  MDNS.update();
#endif
  server.handleClient();
  if (data_buffer.length() > 0)
  {
    String saved_data = data_buffer;
    data_buffer = "";
    return saved_data;
  }
  return "";
}

// Send data to client via WebView MIT app hack
void sendDataToWebView(String data)
{
  String webViewHack = "<script>window.AppInventor.setWebViewString('" + data + "')</script>";
  server.send(200, "text/html", webViewHack);
}

// Start over-the-air update service
void startOTA(const char *OTAName, const char *OTAPass)
{
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPass);
  ArduinoOTA.onStart([]()
                     { Serial.println("Start"); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

// Convert sizes in bytes to KB and MB
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

// Handle request for saving credentials
void handleSave()
{
  if (!server.hasArg("name") || !server.hasArg("password") || server.arg("name") == NULL || server.arg("password") == NULL || server.arg("password").length() < 8)
  {
    server.send_P(200, "text/html", INCORRECT_page);
    return;
  }
  credentials_t credentials;
  strcpy(credentials.name, server.arg("name").c_str());
  strcpy(credentials.pass, server.arg("password").c_str());
  strcpy(credentials.flag, "some");
  writeCredentials(credentials);
  server.send_P(200, "text/html", SUCCESS_page);
  delay(3000);
  ESP.restart();
}

// Convert the file extension to the MIME type
String getContentType(String filename)
{
  if (server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

// Send the right file to the client (if it exists)
bool handleFileRead(String path)
{
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.html";                    // If a folder is requested, send the index file
  String contentType = getContentType(path); // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.open(path, "r") &&
      (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)))
  {                                       // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))        // If there's a compressed version available
      path += ".gz";                      // Use the compressed version
    File file = SPIFFS.open(path, "r");   // Open the file
    server.streamFile(file, contentType); // Send it to the client
    file.close();                         // Close the file again
    Serial.println(String("Sent file: ") + path);
    return true;
  }
  Serial.println(String("File Not Found: ") + path);
  return false; // If the file doesn't exist, return false
}