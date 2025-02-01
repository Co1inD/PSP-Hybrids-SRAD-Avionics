#ifdef SERIALCOM
void send(char* data){

}
void print(){
  
}
void doComReady(){
  Serial.begin(115200);
  while (!Serial);
}
#endif