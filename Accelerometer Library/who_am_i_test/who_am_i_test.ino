#include <Wire.h>

#define DEVICE_ADDRESS 0b0011000  // Device address (0x18)
#define WHO_AM_I_REG  0x0F        // WHO_AM_I register address

// Define custom I2C pins
#define SDA_PIN 41
#define SCL_PIN 40

void setup() {
    uint8_t response;  // Variable to store the WHO_AM_I register value
    Serial.begin(115200);
    
    // Initialize I2C with custom SDA and SCL pins
    Wire.begin(SDA_PIN, SCL_PIN);
  
    Serial.println("Checking I2C connection...");

    // Begin I2C transmission and write the WHO_AM_I register address
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(WHO_AM_I_REG);
    if (Wire.endTransmission() != 0) {
        Serial.println("I2C device not found!");
        while (1);  // Halt execution if device is not found
    }
    
    // Request 1 byte from the device
    Wire.requestFrom(DEVICE_ADDRESS, 1);
    if (Wire.available()) {
        response = Wire.read();
        Serial.print("WHO_AM_I Register Value: 0x");
        Serial.println(response, HEX);
    } else {
        Serial.println("Failed to read WHO_AM_I register.");
    }
}

void loop() {
    // Nothing to do here
}
