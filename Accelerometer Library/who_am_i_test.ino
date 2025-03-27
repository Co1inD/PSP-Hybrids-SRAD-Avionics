#include <Wire.h>

#define i2C_ADDRESS 0x68  
#define WHO_AM_I  0x0F

void setup() {
    uint8_t response; //response when reading who am i register
    Serial.begin(115200);
    Wire.begin();
  
    Serial.println("Checking I2C connection...");

    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(WHO_AM_I_REG);
    if (Wire.endTransmission() != 0) {
        Serial.println("I2C device not found!");
        while (1); 
    }

    Wire.requestFrom(DEVICE_ADDRESS, 1);
    if (Wire.available()) {
        uint8_t response = Wire.read();
        Serial.print("WHO_AM_I Register Value: 0x");
        Serial.println(response, HEX);
    } else {
        Serial.println("Failed to read WHO_AM_I register.");
    }
}

void loop() {

}
