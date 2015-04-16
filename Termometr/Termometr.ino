#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
OneWire ds(2);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int k;

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600); 
}
 
void loop() {

  
  if(k==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Counter");
    lcd.setCursor(0,1);
    lcd.print("Temp");
  }
  k = k +1;
  lcd.setCursor(13,0);
  lcd.print(k);
  
  
byte data[2];
ds.reset(); 
ds.write(0xCC);
ds.write(0x44);
delay(750);
ds.reset();
ds.write(0xCC);
ds.write(0xBE);
data[0] = ds.read(); 
data[1] = ds.read();
int Temp = (data[1]<< 8)+data[0];
Temp = Temp>>4;

lcd.setCursor(13,1);
  lcd.print(Temp);
  
   if(k==999){
    k=0;
  }
  
Serial.println(Temp);
}
