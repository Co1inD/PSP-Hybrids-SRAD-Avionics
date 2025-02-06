#ifdef srad_hardware

// GPIO ASSN
#define FIRE_1 1
#define PYRO_SENSE1 2
#define FIRE_2 3
#define PYRO_SENSE2 4
#define BARO_SCLK 5
#define BARO_SDI 6
#define BARO_SDO 7
#define BARO_CS 8
#define BNO_SCLK 9
#define BNO_SDA 10
#define BNO_INT 11
#define BNO_NRESET 12
#define FLASH_CLK 13
#define FLASH_CS 14
#define FLASH_DO 15
#define FLASH_DI 16
#define ESP_USB_N 19
#define ESP_USB_P 20
#define Flash_CLK 34
#define Flash_CS 35
#define Flash_MISO 36
#define Flash_MOSI 37
#define GPS_EXTINT 42
#define GPS_SCLK 45
#define GPS_SDA 46
#define Buzzer 38

float* getVelocity() {
  // some stuff
}

float* getAcceleration() {
  // some stuff
}

float* getPosition(){
  // some stuff
}
// return altitude in meters with respect to any static reference point
float* getBaro(){
  // some stuff
}
void launchChute(int number) {
  // some stuff
}
void doSensorReady(){
  // some stuff
}
void update() {
  // get all sensor data

  // call the filter
  
  // some stuff
}


#endif