
// Hardware Driver: Test_Board | Colin_Board | Srad_Hardware
#define Test_Board 1
// Comunication library: Serial_Com | Radio_Com
#define Serial_Com 1
// Logging Library: SD_Log | Null_Log
#define Null_Log 1

#define UPDATE_PACKET_LENGTH 50

#define G 9.8               // m/s/s
#define SPEED_OF_SOUND 300  // m/s

TaskHandle_t mainTask;
TaskHandle_t logTask;
TaskHandle_t comTask;



void setup() {
  doSensorReady();
  doComReady();
  doLogReady();
  xTaskCreatePinnedToCore(
      mainLoop, /* Function to implement the task */
      "main loop", /* Name of the task */
      1000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &mainTask,  /* Task handle. */
      1); /* Core where the task should run */
  xTaskCreatePinnedToCore(
      logLoop, /* Function to implement the task */
      "log loop", /* Name of the task */
      1000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &logTask,  /* Task handle. */
      0); /* Core where the task should run */
  xTaskCreatePinnedToCore(
      sendLoop, /* Function to implement the task */
      "send loop", /* Name of the task */
      1000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &comTask,  /* Task handle. */
      0); /* Core where the task should run */
}

void loop(){
  delay(1000);
}

int armed, takenOff, boostDone, sonicOver, paraOneLaunch, paraTwoLaunch;

float paraTwoAlt = -1.0;
int launchTime;
float* oldAlt[10];
float* newAlt[10];
int altIndex;
bool altFull;

char* logData[3000];
char* sendData[3000];
int lengths[3000];
int IDX = 0;

void logLoop(void *parameter){
  int loggedIDX = 0;
  while(true){
    while(IDX != loggedIDX){
      log(logData[loggedIDX], lengths[loggedIDX]);
      loggedIDX++;
      if (loggedIDX == 3000) loggedIDX = 0;
    }
    delay(10);
  }
}
void sendLoop(void *parameter){
  int sentIDX = 0;
  while(true){
    while(IDX != sentIDX){
      log(logData[sentIDX], lengths[sentIDX]);
      sentIDX++;
      if (sentIDX == 3000) sentIDX = 0;
    }
    delay(10);
  }
}

void queueRecord(char* data, int length){
  char* copiedData = (char*)malloc(length);
  memcpy(copiedData, data, length);
  lengths[IDX] = length;
  logData[IDX] = data;  
  sendData[IDX] = copiedData; 
  IDX++;

}

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
void queueRecord(String data){
  const char* cstr = data.c_str();
  int len = 0;
  while(!cstr[len++]);
  char* copiedcstr = (char*)malloc(len);
  memcpy(copiedcstr, cstr, len);
  queueRecord(copiedcstr, len);
}

void mainLoop(void *parameter) {
  while(true){
    int startTime = millis();
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
      if (averageFloat(oldAlt, 10) >= averageFloat(newAlt, 10)) {
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
    int endTime = millis();
    delay(startTime > endTime - 10? 10 - (endTime - startTime ): 0);
  }
}

float averageFloat(float* arr , int len){
  float sum = 0.0f;
  for(int i = 0; i < len; i++){
    sum += arr[i];
  }
  return sum/len;
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
