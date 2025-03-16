#ifdef Serial_Com

#define MAX_INPUT_BUFFER_SIZE 1028

void send(char* data, int length){
  char x;  
  for (int i = 0; i < length; i++){
    
    x = data[i];
    Serial.printf("%02X", x);
  }
  Serial.write("\n");
  Serial.flush();
}
void send(String data){
  Serial.write("02");
  Serial.println(data);
}
void doComReady(){
  Serial.begin(38400);
  while (!Serial);
}
char* receiveBuffer;
int receiveBufferEnd;
/**
* gets up to the next MAX_INPUT_BUFFER_SIZE characters until the next new line
* if a new line is not present or there are no characters,
* returns null
*/
char* receive(){
  if (!Serial.available()) return NULL;
  while (!receiveBuffer && Serial.available()){
    char data = Serial.read();
    if (data != '\n'){
      receiveBuffer = (char*)malloc(MAX_INPUT_BUFFER_SIZE + 1);
      receiveBuffer[0] = data;
      receiveBufferEnd = 1;
    }
  }
  while(Serial.available()){
    char data = Serial.read();
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
#endif