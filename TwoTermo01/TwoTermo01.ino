#include <OneWire.h>

int oneWirePin = 2;

OneWire ds(oneWirePin); 

byte ds_addr0[] = { 
  0x28, 0xFF, 0xBC, 0x03, 0x42, 0x04, 0x00, 0xEB };  // адрес датчика DS18B20 28 FF BC 03 42 04 00 EB
byte ds_addr1[] = { 
  0x28, 0xFF, 0x39, 0x6A, 0x56, 0x14, 0x03, 0x2B };  // адрес датчика DS18B20 28 FF 39 6A 56 14 03 2B

int temp_0 [2]; //arrays to store temeratures
int temp_1 [2];

void read_temp(byte addr[8], int number); //reads temerature on given address and saves it to the array (temp_0 or temp_1)
void send_for_temp(byte addr[8]); //asks for temerature on given address

long previousMillis = 0;        // will store last time DS was updated
long interval = 1000;           // interval at which to read temp (milliseconds)

void setup() {
 Serial.begin (9600); //for debug

ds.search(ds_addr0); //find addresses of 2 devices we have
ds.search(ds_addr1);

send_for_temp(ds_addr0); //ask firs and second for temperature
send_for_temp(ds_addr1);
/* this is more elegant: have to try it
 ds.reset();                  
 ds.skip();                       // tell all sensors on bus
 ds.write(0x44,0);            // to convert temperature

*/
}

void loop() {
//do your cool stuff here


  if (millis() - previousMillis > interval) {
    previousMillis = millis();   
//reading data from old requests:
read_temp(ds_addr0, 0);
read_temp(ds_addr1, 1);

//sending new requests:
send_for_temp(ds_addr0);
send_for_temp(ds_addr1);
  }
//print data from arrays - if it's not updated - will print old   
Serial.print(temp_0[0]); 
Serial.print("."); 
Serial.print(temp_0[1]); 
Serial.println(" ");
Serial.print(temp_1[0]); 
Serial.print("."); 
Serial.print(temp_1[1]); 
}

void send_for_temp(byte addr[8]){
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1);         // start conversion, with parasite power on at the end
}

void read_temp(byte addr[8], int number){
 byte i;
 byte data[12];
 
 ds.reset();
 ds.select(addr);    
 ds.write(0xBE);         // Read Scratchpad

 for (i = 0; i < 9; i++) {           // we need 9 bytes
   data[i] = ds.read();
 }
 
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

 LowByte = data[0];
 HighByte = data[1];
 TReading = (HighByte << 8) + LowByte;
 SignBit = TReading & 0x8000;  // test most sig bit
 if (SignBit) // negative
 {
   TReading = (TReading ^ 0xffff) + 1; // 2's comp
 }
 Tc_100 = 50 * TReading; // multiply by 100 * 0.5  
 Whole = Tc_100 / 100;  // separate off the whole and fractional portions
 Fract = Tc_100 % 100;

 //code we acrually need:
 
 switch (number){
 case 0: 
 temp_0[0] = Whole;
 temp_0[1] = Fract;
 break;
 case 1: 
 temp_1[0] = Whole;
 temp_1[1] = Fract;
 break;
 default: break;
 }
}
