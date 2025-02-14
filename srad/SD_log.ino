#ifdef SD_Log
void doLogReady(){
  // some stuff
}
void log(char* data, int length){
  // some stuff
  free(data);
}
void log(String data){
  // some stuff
  free(data);
}
#endif