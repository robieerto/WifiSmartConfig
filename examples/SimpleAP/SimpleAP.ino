#include <WifiSmartConfig.h>

#define AP_SSID "esp8266AP"
#define AP_PASS "hesielko"

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
  startWifi(WIFI_AP); // Setup Wifi as hotspot
  startSoftAP(AP_SSID, AP_PASS, ipAddresses); // Setup soft AP
  startMDNS(); // Start the mDNS responder for specific hostname
  startHTTP(); // Start HTTP server
}

void loop(void){
  handleClient();
}
