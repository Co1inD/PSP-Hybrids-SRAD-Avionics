
// Hardware Driver
#define testboard 1
#define SERIALCOM 1
#define Null_log 1
#define G 9.8               // m/s/s
#define SPEED_OF_SOUND 300  // m/s
void setup() {
  doSensorReady();
  doComReady();
  doLogReady();
}

int armed, takenOff, boostDone, sonicOver, paraOneLaunch, paraTwoLaunch;

float paraTwoAlt = -1.0;
int launchTime;
float* oldAlt[10];
float* newAlt[10];
int altIndex;
bool altFull;


void loop() {
  update();
  float vel = getVelocityMagnitude();
  float acc = getAccelerationMagnitude();
  float alt = getBaro();
  if (!armed) return;
  if (!takenOff) {
    if (acc > 3 * G) {
      takenOff = millis();
    }
    return;
  }
  if (!boostDone) {
    float* accVec = getAcceleration();
    if (accVec[2] < 0) {
      boostDone = millis();
    }
    free(accVec);
    return;
  }
  if (!sonicOver) {
    if (vel < SPEED_OF_SOUND) {
      sonicOver = millis();
    }
    return;
  }
  if (!paraOneLaunch) {
    if (altIndex == 10) {
      altIndex = 0;
      altFull = millis();
    }
    oldAlt[altIndex] = newAlt[altIndex];
    newAlt[altIndex] = alt;
    altIndex++;
    if (averageFloat(oldAlt) >= averageFloat(newAlt)) {
      paraOneLaunch = millis();
      launchChute(1);
    }
  }
  if(!paraTwoLaunch){
    if (alt < paraTwoAlt){
      paraTwoLaunch = millis();
      launchChute(2);
    }
  }
}

float getAccelerationMagnitude() {
  float* acc = getAcceleration();
  float mag = sqrt(pow(acc[0], 2) + pow(acc[1], 2) + pow(acc[2], 2));
  return mag;
}

float getVelocityMagnitude() {
  float* vel = getVelocity();
  float mag = sqrt(pow(vel[0], 2) + pow(vel[1], 2) + pow(vel[2], 2));
  return mag;
}

char* updatePacket() {
  char* data = (char*)malloc(42);
  data[0] = 0b01;
  data[1] += 0b00000001 * !!armed;
  data[1] += 0b00000010 * !!takenOff;
  data[1] += 0b00000100 * !!boostDone;
  data[1] += 0b00001000 * !!sonicOver;
  data[1] += 0b00010000 * !!paraOneLaunch;
  data[1] += 0b00100000 * !!paraTwoLaunch;
  memcpy(data + 2, &((int)millis()), 4);
  memcpy(data + 6, getAcceleration(), 12);
  memcpy(data + 18, getVelocity(), 12);
  memcpy(data + 30, getPosition(), 12);
  return data;
}
