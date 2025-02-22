#ifdef Barometer
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 9
#define SLP 1013.25 //sea level pressure in hPa

Adafruit_BME280 bme;

void doSensorReady(){
    unsigned status;
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        log("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        log("SensorID was: 0x"); log(bme.sensorID(),16);
        log("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        log("   ID of 0x56-0x58 represents a BMP 280,\n");
        log("        ID of 0x60 represents a BME 280.\n");
        log("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

float getBaro()
{
  return bme.readAltitude(SLP); //1013.25
}


#endif
