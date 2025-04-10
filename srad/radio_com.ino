#ifdef Radio_Com
// uses E220Lib@1.3.5
#ifdef Test_Board
#define AUX 15
#define TXD 17
#define RXD 16
#define M1  18
#define M0  13

#define MAX_BUFFER_SIZE 400

#endif

// When in radio mode, the callsign must be broadcasted on regular intervals
// Create a file called callsign.h and add the following line to it:
// #define FCC_CALL_SIGN "KX0XXX"
// Replace KX0XXX with your callsign
#include "callsign.h"

#define RADIO_CHANNEL 54 // 850.125 + CH = 904.125 MHz
#define MAX_INPUT_BUFFER_SIZE 1028
#define CALL_SIGN_RETRANSMIT_TIME 600000 // 10 mins

void doComReady(){
  pinMode(RXD, INPUT);
  pinMode(TXD, OUTPUT);
  pinMode(AUX, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M0, OUTPUT);

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD, TXD);
  Serial.println("Hello World!");

  while (!Serial);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  delay(2);
  for(int i = 0; digitalRead(AUX) == LOW && i <= 10; i++){
    delay(1);
    if (i == 10){
      Serial.println("ERROR: AUX pin was not brought high in 12ms continuing anyways... (Wiring fault?)");
    }
  }
}
int bufferSize = 0;
void send(char* data, int length){
  if (bufferSize + length >= MAX_BUFFER_SIZE){
    waitForEmpty();
    bufferSize = 0;
  }
  
  autoTransmitCallSign();
  char x;  
  for (int i = 0; i < length; i++){
    x = data[i];
    Serial2.printf("%02X", x);
    Serial.printf("%02X", x);
  }
  Serial.write("\n");
  Serial.flush();
  Serial2.write("\n");
  Serial2.flush();
  bufferSize += length + 1;
}
void waitForEmpty(){
  while(digitalRead(AUX) == LOW){
    delay(1);
  }
}

void send(String data){
  autoTransmitCallSign();
  Serial2.write("02");
  Serial2.println(data);
}

char* receiveBuffer;
int receiveBufferEnd;
/**
* gets up to the next MAX_INPUT_BUFFER_SIZE characters until the next new line
* if a new line is not present or there are no characters,
* returns null
*/
char* receive(){
  waitForEmpty();
  if (!Serial2.available()) return NULL;
  while (!receiveBuffer && Serial2.available()){
    char data = Serial2.read();
    if (data != '\n'){
      receiveBuffer = (char*)malloc(MAX_INPUT_BUFFER_SIZE + 1);
      receiveBuffer[0] = data;
      receiveBufferEnd = 1;
    }
  }
  while(Serial2.available()){
    char data = Serial2.read();
//    while (data == 255){
//      if (!Serial2.available()){
//        return NULL; 
//      }
//      data = Serial2.read();
//    }
    Serial.printf("%03d : %c\n", data, data);
    if(data != '\n'){
      receiveBuffer[receiveBufferEnd++] = data;
    }
    if(data == '\n'){
      receiveBuffer[++receiveBufferEnd] == 0;
      char* ptr = receiveBuffer;
      receiveBuffer = NULL;
      return ptr;
    }
    if (receiveBufferEnd == (MAX_INPUT_BUFFER_SIZE + 1)){
      free(receiveBuffer);
      receiveBuffer = NULL;
      return receive();
    }
  }
}
int nextCallSignTime = 0;
void autoTransmitCallSign(){
  if (millis() > nextCallSignTime){
    transmitCallSign();
    nextCallSignTime = millis() + CALL_SIGN_RETRANSMIT_TIME;
  }
}
void transmitCallSign(){
  Serial2.println(strcat("03 This is ", FCC_CALL_SIGN));
  Serial.println(strcat("03 This is ", FCC_CALL_SIGN));
}
#endif