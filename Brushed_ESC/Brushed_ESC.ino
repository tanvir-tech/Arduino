// Brushed ESC

void setup() {
  pinMode(11,INPUT); //receiver Signal
  pinMode(3,OUTPUT); //mosfet Gate pin
}

void loop() {
  analogWrite(3,map(pulseIn(11,HIGH),1000,2000,0,255));
}
