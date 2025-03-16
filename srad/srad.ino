
// Hardware Driver: Test_Board | Colin_Board | Srad_Hardware
#define Test_Board 1
// Comunication library: Serial_Com | Radio_Com
#define Serial_Com 1
// Logging Library: SD_Log | Null_Log
#define Null_Log 1

#define UPDATE_PACKET_LENGTH 50

#define G 9.8               // m/s/s
#define SPEED_OF_SOUND 300  // m/s

#define ChunkSize 20
#define MaxSendChunks 100
#define MaxLogChunks 100

TaskHandle_t mainTask;
TaskHandle_t logTask;
TaskHandle_t comTask;

SemaphoreHandle_t sendLock;
SemaphoreHandle_t logLock;


struct SizedData {
  int length;
  char* data;
};

int sendQueueChunkSize;
struct SizedData* sendChunkQueue;

int logQueueChunkSize;
struct SizedData* logChunkQueue;


void setup() {
  sendLock = xSemaphoreCreateMutex();
  sendChunkQueue = (SizedData*)malloc(ChunkSize * sizeof(struct SizedData));

  logLock = xSemaphoreCreateMutex();
  logChunkQueue = (SizedData*)malloc(ChunkSize * sizeof(struct SizedData));

  doSensorReady();
  doComReady();
  doLogReady();
  xTaskCreatePinnedToCore(
    mainLoop,    /* Function to implement the task */
    "main loop", /* Name of the task */
    10000,       /* Stack size in words */
    NULL,        /* Task input parameter */
    0,           /* Priority of the task */
    &mainTask,   /* Task handle. */
    1);          /* Core where the task should run */
  xTaskCreatePinnedToCore(
    logLoop,    /* Function to implement the task */
    "log loop", /* Name of the task */
    10000,      /* Stack size in words */
    NULL,       /* Task input parameter */
    1,          /* Priority of the task */
    &logTask,   /* Task handle. */
    0);         /* Core where the task should run */
  xTaskCreatePinnedToCore(
    sendLoop,    /* Function to implement the task */
    "send loop", /* Name of the task */
    10000,       /* Stack size in words */
    NULL,        /* Task input parameter */
    1,           /* Priority of the task */
    &comTask,    /* Task handle. */
    0);          /* Core where the task should run */

}

void destroyChunk(struct SizedData*);
void loop();
void logLoop(void*);
void sendLoop(void*);
void queueSend(char*, int);



void loop() {
  delay(1000);
}

int armed, takenOff, boostDone, sonicOver, paraOneLaunch, paraTwoLaunch;

float paraTwoAlt = -1.0;
int launchTime;
float oldAlt[10];
float newAlt[10];
int altIndex;
bool altFull;

struct SizedData* logData[MaxLogChunks];
struct SizedData* sendData[MaxSendChunks];
int sendHead;
int sendSize;

int logHead;
int logSize;

void logLoop(void* parameter) {
  while (true) {
    while (logSize != 0) {
      xSemaphoreTake(logLock, pdMS_TO_TICKS(9999999));
      struct SizedData* chunk = logData[logHead];
      logHead = --logHead == -1 ? MaxLogChunks - 1:logHead;
      logSize--;
      xSemaphoreGive(logLock);
      for(int i = 0; i<ChunkSize ; i++){
        log(chunk[i].data, chunk[i].length);
      }
      destroyChunk(chunk);
      delay(1);  // avoid setting off watchdog
    }
    delay(10);
  }
}
void sendLoop(void* parameter) {
  while (true) {
    while (sendSize != 0) {
      xSemaphoreTake(sendLock, pdMS_TO_TICKS(9999999));
      struct SizedData* chunk = sendData[sendHead];
      sendHead = --sendHead == -1 ? MaxSendChunks - 1:sendHead;
      sendSize--;
      xSemaphoreGive(sendLock);
      for(int i = 0; i<ChunkSize ; i++){
        send(chunk[i].data, chunk[i].length);
      }
      destroyChunk(chunk);
      delay(1);  // avoid setting off watchdog
    }
    delay(10);
  }
}
/**
* @param data - the data to record (will be freed after transmission)
* @param length - the length of the data to record
*/
void queueRecord(char* data, int length) {
  char* copiedData = (char*)malloc(length);
  memcpy(copiedData, data, length);
  queueLog(data, length);
  queueSend(copiedData, length);
}

void queueLog(char* data, int length){
  logChunkQueue[logQueueChunkSize].length = length;
  logChunkQueue[logQueueChunkSize].data = data;
  logQueueChunkSize++;
  // if the chunk is full of data queue it
  if (logQueueChunkSize == ChunkSize) {
    // Prevent the stack from being read during
    xSemaphoreTake(logLock, pdMS_TO_TICKS(9999999));
    if (logSize == MaxLogChunks) {
      logHead = ++logHead == MaxLogChunks ? 0: logHead;
      struct SizedData* victimChunk = logData[logHead];
      logData[logHead] = logChunkQueue;
      xSemaphoreGive(logLock);
      destroyChunk(victimChunk);
    } else {
      logHead = ++logHead == MaxLogChunks ? 0: logHead;
      logSize++;
      logData[logHead] = logChunkQueue;
      xSemaphoreGive(logLock);
    }
    logChunkQueue = (SizedData*)malloc(ChunkSize * sizeof(struct SizedData));
    logQueueChunkSize = 0;
  }
}

void queueSend(char* data, int length) {
  //struct SizedData* sizedData = malloc(sizeof(struct SizedData));
  sendChunkQueue[sendQueueChunkSize].length = length;
  sendChunkQueue[sendQueueChunkSize].data = data;
  sendQueueChunkSize++;
  // if the chunk is full of data queue it
  if (sendQueueChunkSize == ChunkSize) {
    // Prevent the stack from being read during
    xSemaphoreTake(sendLock, pdMS_TO_TICKS(9999999));
    if (sendSize == MaxSendChunks) {
      sendHead = ++sendHead == MaxSendChunks ? 0: sendHead;
      struct SizedData* victimChunk = sendData[sendHead];
      sendData[sendHead] = sendChunkQueue;
      xSemaphoreGive(sendLock);
      destroyChunk(victimChunk);
    } else {
      sendHead = ++sendHead == MaxSendChunks ? 0: sendHead;
      sendSize++;
      sendData[sendHead] = sendChunkQueue;
      xSemaphoreGive(sendLock);
    }
    sendChunkQueue = (SizedData*)malloc(ChunkSize * sizeof(struct SizedData));
    sendQueueChunkSize = 0;
    // Desynchronize
  }
}

void destroyChunk(struct SizedData* victim) {
  for (int i = 0; i < ChunkSize; i++) {
    free(victim[i].data);
  }
  
  free(victim);
}

void queueRecord(String data) {
  const char* cstr = data.c_str();
  int len = strlen(cstr);
  char* copiedcstr = (char*)malloc(len + 5);
  copiedcstr[0] = 0b10;
  memcpy(copiedcstr + 1, &len, 4);
  memcpy(copiedcstr + 5, cstr, len);
  queueRecord(copiedcstr, len + 5);
}

void mainLoop(void* parameter) {
  while (true) {
    int startTime = millis();
    update();
    float vel = getVelocityMagnitude();
    float acc = getAccelerationMagnitude();
    float alt = getBaro();
    if (!armed) {
    } else if (!takenOff) {
      if (acc > 3 * G) {
        takenOff = millis();
      }

    } else if (!boostDone) {
      float* accVec = getAcceleration();
      if (accVec[2] < 0) {
        boostDone = millis();
      }

    } else if (!sonicOver) {
      if (vel < SPEED_OF_SOUND) {
        sonicOver = millis();
      }

    } else if (!paraOneLaunch) {
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
    } else if (!paraTwoLaunch) {
      if (alt < paraTwoAlt) {
        paraTwoLaunch = millis();
        launchChute(2);
      }
    }
    char* updateP = updatePacket();
    queueRecord(updateP, UPDATE_PACKET_LENGTH);
    int endTime = millis();
    if (endTime - startTime <  10){
      delay(10 - (endTime - startTime));
    }
    else{
      // Serial.println("Running Late!");
    }
    char* datas = receive();
    if (datas) {
      if (datas[0] == 'a' && datas[1] == 'r' && datas[2] == 'm') {
        armed = millis();
      } else if (String(datas).startsWith("disarm")) {
        if (!takenOff) {
          armed = 0;
          queueRecord("Disarmed!\n");
        } else {
          armed = 0;
          takenOff = 0;
          boostDone = 0;
          sonicOver = 0;
          paraOneLaunch = 0;
          paraTwoLaunch = 0;
          queueRecord("Boost already started! disarming anyways!!");
        }
      }
    }
  }
}

float averageFloat(float* arr, int len) {
  float sum = 0.0f;
  for (int i = 0; i < len; i++) {
    sum += arr[i];
  }
  return sum / len;
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
  status += 0b00000001 * 0 != armed;
  status += 0b00000010 * 0 != takenOff;
  status += 0b00000100 * 0 != boostDone;
  status += 0b00001000 * 0 != sonicOver;
  status += 0b00010000 * 0 != paraOneLaunch;
  status += 0b00100000 * 0 != paraTwoLaunch;
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
