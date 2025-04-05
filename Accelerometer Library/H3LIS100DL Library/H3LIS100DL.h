/* 
  H3LIS100DL HEADER FILE 
  v.1 
  3/18/2025
  Louis DeSano
 * @file     H3LIS100DL.h
 */

#ifndef H3LIS100DL_H  // Prevents multiple inclusions of header
#define H3LIS100DL_H  

#include <Arduino.h>
#include <Wire.h>

// Define Registers
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20 // Power mode: Data Rate: Z, Y, X enable
#define CTRL_REG2 0x21 // reboot mem: HPF: HPF configs
#define CTRL_REG3 0x22 // interrupt register
#define CTRL_REG4 0x23 // SPI serial interface select
#define CTRL_REG5 0x24 // Sleep to wake function
#define HP_FILTER_RESET 0x25 // dummy register: zeros accel readings if HPF enabled
#define REFERENCE 0x26 // reference for HPF output
#define STATUS_REG 0x27 //tells if data has been overrun: tells if new data available
#define OUT_X 0x29 // Expressed as two’s complement
#define OUT_Y 0x2B // Expressed as two’s complement
#define OUT_Z 0x2D // Expressed as two’s complement
#define INT1_CFG 0x30 //interrupt generation at low and high g events
#define INT1_SRC 0x31 // read clears the INT1_SRC IA (interrupt active) bit
#define INT1_THS 0x32 
#define INT1_DURATION 0x33 // set  minimum duration of interrupt 1 event to be recognized
// same for interrupt 2 registers
#define INT2_CFG 0x34
#define INT2_SRC 0x35
#define INT2_THS 0x36
#define INT2_DURATION 0x37

#define REGISTER_SIZE 1 // registers are 1 byte/ 8 bits long 
#define WHO_AM_I_RESPONSE 0x32

class H3LIS100DL {
public:
    
    /*!
     * @brief       Constructor for the accelerometer class.
     * @param addr I2C address of the accelerometer.
     */
    H3LIS100DL(uint8_t addr);

    /*!
     * @brief     Checks communication, Initializes i2C, Initializes data collection.
     * @param SDA SDA pin
     * @param SCL SCL pin
     * @return    `true` if the device is detected, `false` otherwise.
     */
    bool H3LIS100DL_begin(int SDA, int SCL);  

  
    /*!
     * @brief     Reads the value from a specific register.
     * @param reg Register address to read from.
     * @return    The 8-bit value stored in the register.
     */
    uint8_t readRegister(uint8_t reg);



    /*!
     * @brief         Writes a message to a register
     * @param reg     Register address to write to
     * @param message To be written to register.
     * @return        True if successful, False if failed.
     */
    bool writeRegister(uint8_t reg, uint8_t message);


   /*!
     * @brief   Reads raw acceleration data from the X, Y, and Z axes.
     * @param x Reference to store X-axis acceleration.
     * @param y Reference to store Y-axis acceleration.
     * @param z Reference to store Z-axis acceleration.
     */
    void H3LIS100DL_readXYZ(int16_t *x, int16_t *y, int16_t *z);

private:
    uint8_t _address;  // Stores the I2C address of the accelerometer

};

#endif 
