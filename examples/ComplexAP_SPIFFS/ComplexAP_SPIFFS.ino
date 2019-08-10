#include <WifiSmartConfig.h>

#define AP_SSID "espAP"
#define AP_PASS "hesielko"
#define OTA_NAME "esp"
#define OTA_PASS "hesielko123"

ipAddresses_t ipAddresses = {
  IPAddress(192, 168, 4, 1), // Local IP
  IPAddress(192, 168, 4, 0), // Gateway
  IPAddress(255, 255, 255, 0) // Subnet Mask
};

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  // clearCredentials(); // Clear credentials

  // Save multiple Wifi credentials
  //wifiMulti.addAP("ssid1", "password1");
  //wifiMulti.addAP("ssid2", "password2");

  // Setup Wifi, WIFI_STA - only station,
  //  WIFI_AP - only hotspot, WIFI_AP_STA - both
  startWifi(WIFI_AP_STA); // Connect to one of the saved Wifi SSID's
  startSoftAP(AP_SSID, AP_PASS, ipAddresses); // Setup soft AP
  startMDNS(); // Start the mDNS responder for specific hostname
  startSPIFFS(); // Start the SPI Flash Files System
  startHTTP(); // Start HTTP server
  startOTA(OTA_NAME, OTA_PASS); // Start over-the-air update service
}

void loop(void){
  ArduinoOTA.handle();
  handleClient();
}

