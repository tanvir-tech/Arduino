char cmd;
int firePin=12;

void setup() {
  Serial.begin(9600);//Any type serial communication--- on the TX-RX pin
  pinMode(firePin,OUTPUT);
}

void loop() {

if(Serial.available()!=0){
  cmd = Serial.read();
  Serial.println(cmd);
  }

if(cmd=='a'){
  digitalWrite(firePin,HIGH);
  Serial.println("Fired");
  }

if(cmd=='b'){
  digitalWrite(firePin,LOW);
  Serial.println("Shut down");
  }

}
