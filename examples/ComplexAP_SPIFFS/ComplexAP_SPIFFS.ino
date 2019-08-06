#include <WifiSmartConfig.h>

#define AP_SSID "esp8266AP"
#define OTA_NAME "esp8266"
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
  startWifi(WIFI_AP_STA); // Connect to one of the saved Wifi SSID's
  startSoftAP(AP_SSID); // Setup soft AP
  startMDNS(); // Start the mDNS responder for specific hostname
  startSPIFFS(); // Start the SPI Flash Files System
  startHTTP(); // Start HTTP server
  startOTA(OTA_NAME, OTA_PASS); // Start over-the-air update service
}

void loop(void){
  MDNS.update();
  ArduinoOTA.handle();
  server.handleClient();
}
