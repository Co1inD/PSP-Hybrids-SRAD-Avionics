
#include <Arduino.h>
#include <Wire.h>
#include "MS5607.h"  // Our updated library
MS5607 baro; // Create MS5607 object
void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("MS5607 I2C Test on custom pins (SDA=17, SCL=18)");
  // 1) Initialize I2C bus on custom pins at 400kHz
  Wire.begin(48, 42, 400000);
  // 2) Attempt to initialize MS5607
  if (!baro.begin()) {
    Serial.println("Error communicating with MS5607, check wiring/address!");
    while (true) { delay(10); }
  }
  Serial.println("MS5607 init successful!");
  // Optionally, set oversampling ratio
  // baro.setOSR(4096); // default
}
void loop()
{
  // We must call readDigitalValue() before getTemperature()/getPressure()
  if (baro.readDigitalValue())
  {
    float tempC     = baro.getTemperature(); // Board Temperture  
    float pressure  = baro.getPressure();
    float altitudeM = baro.getAltitude();
    Serial.print("Temp: ");
    Serial.print(tempC, 2);
    Serial.print(" C, Press: ");
    Serial.print(pressure, 2);
    Serial.print(" mbar, Alt: ");
    Serial.print(altitudeM, 2);
    Serial.println(" m");
  }
  else
  {
    Serial.println("Error: readDigitalValue() failed!");
  }
  delay(1000);
}
