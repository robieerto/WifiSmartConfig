#include <WifiSmartConfig.h>

#define AP_SSID "espAP"
#define AP_PASS nullptr

ipAddresses_t ipAddresses = {
    IPAddress(192, 168, 4, 1),  // Local IP
    IPAddress(192, 168, 4, 0),  // Gateway
    IPAddress(255, 255, 255, 0) // Subnet Mask
};

credentials_t savedData;
bool isDataSaved;

void setup(void)
{
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  startAddrCredentials = 0;

  // clearCredentials(); // Clear credentials

  savedData = readCredentials();
  isDataSaved = areCredentialsSaved(savedData);
  if (isDataSaved == false)
  {
    startWifi(WIFI_AP);                         // Setup Wifi as hotspot
    startSoftAP(AP_SSID, AP_PASS, ipAddresses); // Setup soft AP
    startHTTP();                                // Start HTTP server
  }
  else
  {
    wifiMulti.addAP(savedData.name, savedData.pass);
    startWifi(WIFI_STA); // Connect to saved Wifi SSID
  }
}

void loop(void)
{
  if (isDataSaved == false)
  {
    handleClient();
  }
}
