#ifdef Null_Log
void doLogReady(){
  // no stuff
}
void log(char* data, int length){
  free(data);
}
void log(String data){
}
#endif