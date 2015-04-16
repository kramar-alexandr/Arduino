//#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
OneWire  ds(10);  // линия 1-Wire будет на pin 10
int k;
void setup()
{
 lcd.init();                      // initialize the lcd 
 // Print a message to the LCD.
 lcd.backlight();
 lcd.print("Hello, world!");
 Serial.begin(9600);
}
void loop()
{
  
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
  delay(1000);
  
 
  
  
}
