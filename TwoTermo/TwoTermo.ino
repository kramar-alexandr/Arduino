#include <OneWire.h>
#include <DallasTemperature.h>
 
#define ONE_WIRE_BUS 2


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
DeviceAddress Thermometer1 = { 
  0x28, 0xFF, 0xBC, 0x03, 0x42, 0x04, 0x00, 0xEB };  // адрес датчика DS18B20 28 FF BC 03 42 04 00 EB
DeviceAddress Thermometer2 = { 
  0x28, 0xFF, 0x39, 0x6A, 0x56, 0x14, 0x03, 0x2B };  // адрес датчика DS18B20 28 FF 39 6A 56 14 03 2B
 
void setup() {
 
  sensors.begin();
  sensors.setResolution(Thermometer1, 10);
  sensors.setResolution(Thermometer2, 10);
 
  Serial.begin(9600);
}
 
void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.println(tempC);
}
void loop() {
 
  sensors.requestTemperatures();
  Serial.print("Sensor1  ");
  printTemperature(Thermometer1);
 
  Serial.print("Sensor2  ");
  printTemperature(Thermometer2);
}
