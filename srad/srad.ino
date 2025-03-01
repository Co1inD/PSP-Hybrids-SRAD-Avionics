
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
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &mainTask,  /* Task handle. */
      1); /* Core where the task should run */
  xTaskCreatePinnedToCore(
      logLoop, /* Function to implement the task */
      "log loop", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &logTask,  /* Task handle. */
      0); /* Core where the task should run */
  xTaskCreatePinnedToCore(
      sendLoop, /* Function to implement the task */
      "send loop", /* Name of the task */
      10000,  /* Stack size in words */
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
float oldAlt[10];
float newAlt[10];
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
      delay(1); // avoid setting off watchdog
    }
    delay(10);
  }
}
void sendLoop(void *parameter){
  int sentIDX = 0;
  while(true){
    while(IDX != sentIDX){
      send(sendData[sentIDX], lengths[sentIDX]);
      sentIDX++;
      if (sentIDX == 3000) sentIDX = 0;
      delay(1); // avoid setting off watchdog
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
  if (IDX == 3000) IDX = 0;
}

void queueRecord(String data){
  const char* cstr = data.c_str();
  int len = strlen(cstr);
  char* copiedcstr = (char*)malloc(len + 5);
  copiedcstr[0] = 0b10;
  memcpy(copiedcstr + 1, &len, 4);
  memcpy(copiedcstr + 5, cstr, len);
  queueRecord(copiedcstr, len + 5);
}

void mainLoop(void *parameter) {
  while(true){
    int startTime = millis();
    update();
    float vel = getVelocityMagnitude();
    float acc = getAccelerationMagnitude();
    float alt = getBaro();
    if (!armed){}
    else if (!takenOff) {
      if (acc > 3 * G) {
        takenOff = millis();
      }
      
    }
    else if (!boostDone) {
      float* accVec = getAcceleration();
      if (accVec[2] < 0) {
        boostDone = millis();
      }
      
    }
    else if (!sonicOver) {
      if (vel < SPEED_OF_SOUND) {
        sonicOver = millis();
      }
      
    }
    else if (!paraOneLaunch) {
      if (altIndex == 10) {
        altIndex = 0;
        altFull = millis();
      }
      oldAlt[altIndex] = newAlt[altIndex];
      newAlt[altIndex] = alt;
      altIndex++;
      if (altFull && averageFloat(oldAlt, 10) >= averageFloat(newAlt, 10)) {
        paraOneLaunch = millis();
        launchChute(1);
      }
    }
    else if(!paraTwoLaunch){
      if (alt < paraTwoAlt){
        paraTwoLaunch = millis();
        launchChute(2);
      }
    }
    char* updateP = updatePacket();
    queueRecord(updateP, UPDATE_PACKET_LENGTH);
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
  char* data = (char*)malloc(UPDATE_PACKET_LENGTH);
  data[0] = 0b01;
  char status = 0;
  status += 0b00000001 * 0!=armed;
  status += 0b00000010 * 0!=takenOff;
  status += 0b00000100 * 0!=boostDone;
  status += 0b00001000 * 0!=sonicOver;
  status += 0b00010000 * 0!=paraOneLaunch;
  status += 0b00100000 * 0!=paraTwoLaunch;
  data[1] = status;
  int timeTemp = (int)millis();
  memcpy(data + 2, &(timeTemp), 4);
  memcpy(data + 6, getAcceleration(), 12);
  memcpy(data + 18, getVelocity(), 12);
  memcpy(data + 30, getPosition(), 12);
  memcpy(data + 42, getOrientation(), 8);
  delay(100);
  return data;
}
