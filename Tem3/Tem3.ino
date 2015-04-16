#include <OneWire.h>
OneWire ds(2);

byte ds_addr0[] = { 
  0x28, 0xFF, 0xBC, 0x03, 0x42, 0x04, 0x00, 0xEB };  // адрес датчика DS18B20 28 FF BC 03 42 04 00 EB
byte ds_addr1[] = { 
  0x28, 0xFF, 0x39, 0x6A, 0x56, 0x14, 0x03, 0x2B };  // адрес датчика DS18B20 28 FF 39 6A 56 14 03 2B

void setup() {
Serial.begin(9600); 
}
 
void loop() {
 
byte data[2];
ds.reset(); 
ds.select(ds_addr0);
//ds.write(0xCC);
ds.write(0x44);

byte data1[2];
ds.reset(); 
ds.select(ds_addr1);
//ds.write(0xCC);
ds.write(0x44);


delay(750);
ds.reset();
ds.select(ds_addr0);
//ds.write(0xCC);
ds.write(0xBE);
data[0] = ds.read(); 
data[1] = ds.read();
int Temp = (data[1]<< 8)+data[0];
float ResTemp = (float)(Temp>>2)*0.25;


ds.reset();
ds.select(ds_addr1);
//ds.write(0xCC);
ds.write(0xBE);
data1[0] = ds.read(); 
data1[1] = ds.read();
int Temp1 = (data1[1]<< 8)+data1[0];
float ResTemp1 = (float)(Temp1>>2)*.25;

Serial.print("T1 ");
Serial.println(ResTemp);
Serial.print("T2 ");
Serial.println(ResTemp1);

}
