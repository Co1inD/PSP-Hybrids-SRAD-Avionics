#include "H3LIS100DL.h"
#define SDA_PIN 41
#define SCL_PIN 40

H3LIS100DL acc(0b0011000);
int16_t x, y, z;
uint8_t dataO;
uint8_t dataA;

void setup() {
    Serial.begin(115200);

    // Initialize the accelerometer and check if it responds correctly.
    if (!acc.H3LIS100DL_begin(SDA_PIN, SCL_PIN)) {
        Serial.println("H3LIS100DL not found");
        while(1); // wait if the sensor is not detected
    }
}

void loop() {
    acc.H3LIS100DL_readXYZ(&x, &y, &z);
    Serial.printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    delay(20);
}