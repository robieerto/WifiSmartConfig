#include <WifiSmartConfig.h>

#define HOSTNAME "esp8266"
#define OTA_PASS "hesielko"

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  // Save multiple Wifi credentials
  //wifiMulti.addAP("ssid1", "password1");
  //wifiMulti.addAP("ssid2", "password2");

  // Setup Wifi, WIFI_STA - only station,
  //  WIFI_AP - only hotspot, WIFI_AP_STA - both
  startWifi(WIFI_AP); // Connect to one of the saved Wifi SSID's
  startSoftAP(); // Setup soft AP
  startMDNS(HOSTNAME); // Start the mDNS responder for specific hostname
  startSPIFFS(); // Start the SPI Flash Files System
  startHTTP(); // Start HTTP server
  // startOTA(HOSTNAME, OTA_PASS); // Start over-the-air update service
}

void loop(void){
  MDNS.update();
  // ArduinoOTA.handle();
  server.handleClient();
}