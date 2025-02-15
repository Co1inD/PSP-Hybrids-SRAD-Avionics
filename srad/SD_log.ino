#ifdef SD_Log
#include <SPI.h>
#include <SD.h>
void doLogReady()
{
  File myFILE;
  myFile = SD.open("Output_File.txt", FILE_APPEND);
  myFile.write("Test Data\n");
  myFile = SD.close();
}
void log(char* data, int length){
  // some stuff
  myFile = SD.open("Output_File.txt", FILE_APPEND);
  //logging data
  free(data);
}
void log(String data){
  myFile = SD.open("Output_File.txt", FILE_APPEND);
  //logging data
  free(data);
}
#endif
