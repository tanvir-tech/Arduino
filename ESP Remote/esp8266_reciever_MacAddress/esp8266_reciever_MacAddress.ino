#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.macAddress());
}

void loop() {
  // 68:C6:3A:D6:5D:33
  Serial.println(WiFi.macAddress());
  delay(1000);
}