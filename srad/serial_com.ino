#ifdef Serial_Com
void send(char* data, int length){
  char x;

  for (int i = 0; i < length; i++){

    x = data[i];
    if ( (x>='0' && x<='9')? (x-'0'):((x>='A' && x<= 'F')?((x-'A')+10):(0)) ){
      Serial.write((char)x);
    }
    Serial.write("\n");
  }
  free(data);
}
void send(String data){
  Serial.write("02");
  Serial.println(data);
}
void doComReady(){
  Serial.begin(115200);
  while (!Serial);
}
#endif