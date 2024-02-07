/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-one-to-many-esp32-esp8266/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <espnow.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define SIGNAL_TIMEOUT 1000  // This is signal timeout in milli seconds. We will reset the data if no signal

unsigned long lastRecvTime = 0;

//Structure example to receive data
//Must match the sender structure
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
PacketData receiverData;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int led1 = 15;
int led2 = 16;
int led3 = 17;
int led4 = 18;
int led5 = 19;
int led6 = 21;
int led7 = 22;
int led8 = 23;

//Assign default input received values
void setInputDefaultValues() {
  // The middle position for joystick. (254/2=127)
  receiverData.lxAxisValue = 127;
  receiverData.lyAxisValue = 127;
  receiverData.rxAxisValue = 127;
  receiverData.ryAxisValue = 127;

  receiverData.switch1Value = LOW;
  receiverData.switch2Value = LOW;
  receiverData.switch3Value = LOW;
  receiverData.switch4Value = LOW;

  receiverData.switch5Value = LOW;
  receiverData.switch6Value = LOW;
  receiverData.switch7Value = LOW;
  receiverData.switch8Value = LOW;
}

void mapAndWriteValues() {
  servo1.write(map(receiverData.lxAxisValue, 0, 254, 0, 180));
  servo2.write(map(receiverData.lyAxisValue, 0, 254, 0, 180));
  servo3.write(map(receiverData.rxAxisValue, 0, 254, 0, 180));
  servo4.write(map(receiverData.ryAxisValue, 0, 254, 0, 180));

  digitalWrite(led1, receiverData.switch1Value);
  digitalWrite(led2, receiverData.switch2Value);
  digitalWrite(led3, receiverData.switch3Value);
  digitalWrite(led4, receiverData.switch4Value);

  digitalWrite(led5, receiverData.switch5Value);
  digitalWrite(led6, receiverData.switch6Value);
  digitalWrite(led7, receiverData.switch7Value);
  digitalWrite(led8, receiverData.switch8Value);
}


//callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len == 0) {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  Serial.print("lx: ");
  Serial.println(receiverData.lxAxisValue);
  Serial.print("LED1_GPIO-15: ");
  Serial.println(receiverData.switch1Value);
  mapAndWriteValues();
  lastRecvTime = millis();
}

void setUpPinModes() {
  servo1.attach(27);
  servo2.attach(26);
  servo3.attach(25);
  servo4.attach(33);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  setInputDefaultValues();
  mapAndWriteValues();
}

void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  //Check Signal lost.
  unsigned long now = millis();
  if (now - lastRecvTime > SIGNAL_TIMEOUT) {
    setInputDefaultValues();
    mapAndWriteValues();
    // Serial.println(receiverData.switch1Value);
  }
}