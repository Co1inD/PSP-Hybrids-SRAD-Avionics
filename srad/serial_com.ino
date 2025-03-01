#ifdef Serial_Com
void send(char* data, int length){
  char x;  
  for (int i = 0; i < length; i++){
    
    x = data[i];
    Serial.printf("%02X", x);
  }
  Serial.write("\n");
  Serial.flush();
  free(data);
}
void send(String data){
  Serial.write("02");
  Serial.println(data);
}
void doComReady(){
  Serial.begin(9600);
  while (!Serial);
}
#endif