#ifdef SERIALCOM
void send(char* data, int length){
  char x;

  for (int i = 0; i < length; i++){

    x = data[i];
    if ( (x>='0' && x<='9')? (x-'0'):((x>='A' && x<= 'F')?((x-'A')+10):(0)) ){
      Serial.write(x);
    }
    Serial.write("\n");
}
}
void log(String data){
  Serial.write("02");
  Serial.write(data);
  Serial.write("\n");
}
void doComReady(){
  Serial.begin(115200);
  while (!Serial);
}
#endif