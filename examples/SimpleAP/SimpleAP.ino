#include <WifiSmartConfig.h>

#define AP_SSID "esp8266AP"

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  startWifi(WIFI_AP); // Setup Wifi as hotspot
  startSoftAP(AP_SSID); // Setup soft AP
  startMDNS(); // Start the mDNS responder for specific hostname
  startHTTP(); // Start HTTP server
}

void loop(void){
  MDNS.update();
  server.handleClient();
}
