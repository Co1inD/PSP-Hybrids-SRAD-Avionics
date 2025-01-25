
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

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 9

Adafruit_BME280 bme;

void doSetupBaro(){
    unsigned status;
    
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

void getBaro(){
  
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(1013.25));
    Serial.println(" m");
}


void setup() {
  
  
  Serial.begin(9600);
  while(!Serial);
  // GPS

  // Barometer
  doSetupBaro();
  // IMU

  // Hi G
  
  
}

void loop() {
  getBaro();
}







char* updatePacket(){
  char *data = (char *)malloc(42);
  // TODO
  return data;
}

// Kalman filter code
// don't ask questions you don't need the answers to

// State variables
float x[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // [x, y, z, vx, vy, vz]

// Matrices
float F[6][6] = {
  {1, 0, 0, 0.01, 0, 0}, {0, 1, 0, 0, 0.01, 0}, {0, 0, 1, 0, 0, 0.01},
  {0, 0, 0, 1, 0, 0},    {0, 0, 0, 0, 1, 0},    {0, 0, 0, 0, 0, 1}
};

float B[6][3] = {
  {0.00005, 0, 0}, {0, 0.00005, 0}, {0, 0, 0.00005},
  {0.01, 0, 0},    {0, 0.01, 0},    {0, 0, 0.01}
};

float H[3][6] = {
  {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}
};

float P[6][6] = {0}; // Initialize state covariance to zero
float Q[6][6] = {0}; // Process noise covariance
float R[3][3] = { // Measurement noise covariance
  {0.1, 0, 0}, {0, 0.1, 0}, {0, 0, 0.1}
};

float K[6][3]; // Kalman gain

void kalmanUpdate(float z[3]) {
  // Compute Kalman gain: K = P * H^T / (H * P * H^T + R)
  float PHt[6][3] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 6; k++) {
        PHt[i][j] += P[i][k] * H[j][k];
      }
    }
  }

  float S[3][3] = {0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 6; k++) {
        S[i][j] += H[i][k] * PHt[k][j];
      }
      S[i][j] += R[i][j];
    }
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      K[i][j] = PHt[i][j] / S[j][j];
    }
  }

  // Update state: x = x + K * (z - H * x)
  float y[3] = {0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      y[i] -= H[i][j] * x[j];
    }
    y[i] += z[i];
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      x[i] += K[i][j] * y[j];
    }
  }

  // Update covariance: P = (I - K * H) * P
  float KH[6][6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 3; k++) {
        KH[i][j] += K[i][k] * H[k][j];
      }
    }
  }

  float I_KH[6][6] = {0};
  for (int i = 0; i < 6; i++) {
    I_KH[i][i] = 1.0;
    for (int j = 0; j < 6; j++) {
      I_KH[i][j] -= KH[i][j];
    }
  }

  float newP[6][6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        newP[i][j] += I_KH[i][k] * P[k][j];
      }
    }
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      P[i][j] = newP[i][j];
    }
  }
}

void kalmanPredict(float accel[3]) {
  // Predict state: x = F * x + B * u
  float Fx[6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      Fx[i] += F[i][j] * x[j];
    }
  }

  float Bu[6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      Bu[i] += B[i][j] * accel[j];
    }
  }

  for (int i = 0; i < 6; i++) {
    x[i] = Fx[i] + Bu[i];
  }

  // Predict covariance: P = F * P * F^T + Q
  float FP[6][6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        FP[i][j] += F[i][k] * P[k][j];
      }
    }
  }

  float FPFt[6][6] = {0};
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 6; k++) {
        FPFt[i][j] += FP[i][k] * F[j][k];
      }
    }
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      P[i][j] = FPFt[i][j] + Q[i][j];
    }
  }
}
