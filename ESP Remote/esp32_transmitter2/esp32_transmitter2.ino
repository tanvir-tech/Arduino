/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-one-to-many-esp32-esp8266/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = { 0x68, 0xC6, 0x3A, 0xD6, 0x5D, 0x33 };
// uint8_t broadcastAddress2[] = {0x68, 0xC6, 0x3A, 0xD6, 0x5D, 0x33};
// uint8_t broadcastAddress3[] = {0x68, 0xC6, 0x3A, 0xD6, 0x5D, 0x33};

typedef struct PacketData {
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;

  byte switch1Value;
  byte switch2Value;
  byte switch3Value;
  byte switch4Value;
  byte switch5Value;
  byte switch6Value;
  byte switch7Value;
  byte switch8Value;
};
PacketData data;
//This function is used to map 0-4095 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick.
//Jotstick values range from 0-4095. But its center value is not always 2047. It is little different.
//So we need to add some deadband to center value. in our case 1800-2200. Any value in this deadband range is mapped to center 127.
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse) {
  if (value >= 2200) {
    value = map(value, 2200, 4095, 127, 254);
  } else if (value <= 1800) {
    value = (value == 0 ? 0 : map(value, 1800, 0, 127, 0));
  } else {
    value = 127;
  }

  if (reverse) {
    value = 254 - value;
  }
  Serial.println(value);
  return value;
}

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
}

void loop() {
  data.lxAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(32), false);
  data.lyAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(33), false);
  data.rxAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(34), false);
  data.ryAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(35), false);
  data.switch1Value = !digitalRead(15);
  data.switch2Value = !digitalRead(16);
  data.switch3Value = !digitalRead(17);
  data.switch4Value = !digitalRead(18);
  data.switch5Value = !digitalRead(19);
  data.switch6Value = !digitalRead(21);
  data.switch7Value = !digitalRead(22);
  data.switch8Value = !digitalRead(23);

  esp_err_t result = esp_now_send(0, (uint8_t *)&data, sizeof(PacketData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(10);
}