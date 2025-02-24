#ifdef Srad_Hardware

// GPIO ASSN
#define FIRE_1 1
#define PYRO_SENSE1 2
#define FIRE_2 3
#define PYRO_SENSE2 4
#define BARO_SCLK 42
#define BARO_SDI 48
#define BARO_SDO 47
#define BARO_CS 5
#define BNO_SCLK 18
#define BNO_SDA 17
#define BNO_NRESET 12
#define FLASH_CLK 13
#define FLASH_CS 14
#define FLASH_DO 15
#define FLASH_DI 16
#define ESP_USB_N 19
#define ESP_USB_P 20
#define FLASH_CLK 13
#define FLASH_CS 14
#define FLASH_MISO 15
#define FLASH_MOSI 16
#define GPS_EXTINT 34
#define GPS_SCLK 35
#define GPS_SDA 36
#define Buzzer 33

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
float getBaro(){
  // some stuff
}

float* getOrientation(){
  // some stuff
}

void launchChute(int number) {
  // Set GPIO1 and GPIO3 as output setup
  pinMode(FIRE_1, OUTPUT);
  pinMode(FIRE_2, OUTPUT);
  // Immediately pull high to ensure channels do not fire
  digitalWrite(FIRE_1, HIGH);
  digitalWrite(FIRE_2, HIGH);
  //End of Setup

  //PULLING GPIO PIN 1 & 3 LOW FIRES PYRO (when safety jumper is installed)
  // Pull GPIO1 and GPIO3 LOW 
  digitalWrite(FIRE_1, LOW);
  delay(1000); //Wait 1 second
  digitalWrite(FIRE_1, HIGH);

  delay(5000); // Wait 5 seconds 
  
  digitalWrite(FIRE_2, LOW);
  delay(1000); //Wait 1 second
  digitalWrite(FIRE_2, HIGH);
}

void doSensorReady(){
  // some stuff
}

//Produces a buzzer pattern depending on what mode is passed in. 
void buzzer(int mode)
{
  pinMode(BUZZER, OUTPUT);
  if(mode == 0) //idle
  {
    digitalWrite(BUZZER, LOW);
  }
  if(mode == 1) //ready
  {
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    digitalWrite(BUZZER,LOW);
  }
  if(mode == 2) //armed
  {
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    digitalWrite(BUZZER,LOW);
    delay(1000);
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    digitalWrite(BUZZER,LOW);
  }
  if(mode == 3) //landed
  {
    digitalWrite(BUZZER, HIGH);
    delay(10000);
     digitalWrite(BUZZER,LOW);
  }
}
void update() {
  // get all sensor data

  // call the filter
  
  // some stuff
}


#endif
