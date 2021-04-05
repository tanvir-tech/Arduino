#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String MACadd = "98:D3:32:11:31:37";
uint8_t address[6]  = {0x98, 0xD3, 0x32, 0x11, 0x31, 0x37};
//uint8_t address[6]  = {0x11, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String name = "HC-05";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32test", true);
  SerialBT.setPin(pin);
  Serial.println("The device started in master mode, make sure remote BT device is on!");

  connected = SerialBT.connect(name);   //this code is for -> only one time connection

  if (connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }

  //  if (SerialBT.disconnect()) {Serial.println("Disconnected Succesfully!");}

}

void loop() {
  //  if (Serial.available()) { //Serial monitor section
  //    SerialBT.write(Serial.read());
  //  }

  if (SerialBT.available()) { //BT reciever section
    Serial.write(SerialBT.read());
    Serial.write("Some_Message");
  } else {
    Serial.println("No_Message !");
    delay(20);
  }




  if (connected) {
    Serial.println("Connected Succesfully!");
  }



  delay(20);









}
