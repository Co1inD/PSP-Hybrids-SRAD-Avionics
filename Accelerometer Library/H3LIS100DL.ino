#include "H3LIS100DL.h"

H3LIS100DL acc(0b0011000);
int16_t x, y, z;

void setup() {
    Serial.begin(115200);

    // Initialize the accelerometer and check if it responds correctly.
    if (!acc.H3LIS100DL_begin()) {
        Serial.println("H3LIS100DL not found");
        while(1); // wait if the sensor is not detected
    }
}

void loop() {
    acc.H3LIS100DL_readXYZ(&x, &y, &z);
    Serial.printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    delay(100);
}