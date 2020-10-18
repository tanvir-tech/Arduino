#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);


void setup() {
  lcd.begin();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Nusrat Jahan");
  lcd.setCursor(0,1);
  lcd.print("Swarna");




}
