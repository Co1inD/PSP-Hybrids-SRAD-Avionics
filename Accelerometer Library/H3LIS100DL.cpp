#include "H3LIS100DL.h"

// Constructor class: takes in slave address and saves as attribute of sensor's class
H3LIS100DL::H3LIS100DL(uint8_t i2Caddress) {
    _address = i2Caddress;
}

// begins i2c connection and checks it is valid
bool H3LIS100DL::H3LIS100DL_begin(int SDA, int SCL) {
    Wire.begin(SDA, SCL);
    
    //check i2C connection with accelerometer
    if (this->readRegister(WHO_AM_I) == WHO_AM_I_RESPONSE) {
       
        //initialize x y z collection
        if(this->writeRegister(CTRL_REG1, 0b00100111)) {
            return true;
        }
    }

    return false;

    //if no valid connection end wire and print error
    else {
        Wire.end();
        Serial.println("H3LIS100DL not found");
        return false;
    }
}

// Writes message to a register
bool H3LIS100DL::writeRegister(uint8_t reg, uint8_t message) {
    uint8_t error; // error flag for failed transmission

    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(message);
    error = Wire.endTransmission();

    // Check transmission was successful
    if (error != 0) {
        Serial.println("Write to Register Failed");
        return false;
    }

    return true;
}


//reads a register
uint8_t H3LIS100DL::readRegister(uint8_t reg) {
    uint8_t error; // error flag for failed transmission
    uint8_t dataReceived; //received byte 

    Wire.beginTransmission(_address); // begin transmission to sensor
    Wire.write(reg); //call register for data
    error = Wire.endTransmission(); //end transmission, check for NACK or errors
    
    // Check sucessful transmission
    if (error != 0) {
        Serial.println("Register Read Failed");
        return 255; // error case return
    }
    
    // request data and check correct number of bytes is recieved
    Wire.requestFrom(_address, REGISTER_SIZE); 
    if (Wire.available() == REGISTER_SIZE) {
        dataReceived = Wire.read(); 
        return dataReceived;
    }

    Serial.println("Not Enough Data Recieved");
    return 255; // error case return
}

//reads xyz acceleration data
void H3LIS100DL::H3LIS100DL_readXYZ(int16_t *x, int16_t *y, int16_t *z) {
    //raw values from registers (twos complement binary)
    uint8_t x_raw; 
    uint8_t y_raw;
    uint8_t z_raw;

    //read raw acceleration data from registers
    x_raw = this->readRegister(OUT_X);
    y_raw = this->readRegister(OUT_Y);
    z_raw = this->readRegister(OUT_Z);

    // cast raw values to int to make them decimal and resolve twos complement
    // gives signed decimal output in g's
    
    
    *x = (x_raw < 128) ? x_raw : x_raw - 256;
    *y = (y_raw < 128) ? y_raw : y_raw - 256;
    *z = (z_raw < 128) ? z_raw : z_raw - 256;
}