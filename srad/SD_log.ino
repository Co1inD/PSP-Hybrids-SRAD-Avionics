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
  for(int i = 0; i < length; i++)
    {
        myFile.write("%s\n", data[i]);
    }
  //logging data
  free(data);
}
void log(String data){
  myFile = SD.open("Output_File.txt", FILE_APPEND);
  myFile.write("%s\n", data);
  free(data);
}
#endif
