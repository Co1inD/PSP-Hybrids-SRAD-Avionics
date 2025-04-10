#include "E220.h"
#include <SoftwareSerial.h>

#define AUX 2
#define TXD 4
#define RXD 3
#define M1  5
#define M0  6

#include "callsign.h"

#define RADIO_CHANNEL 54 // 850.125 + CH = 94.125 MHz
#define MAX_INPUT_BUFFER_SIZE 1028
#define CALL_SIGN_RETRANSMIT_TIME 600000 // 10 mins



SoftwareSerial radioSerial = SoftwareSerial(RXD, TXD);

void setup() {
  pinMode(RXD, INPUT);
  pinMode(TXD, OUTPUT);
  
  radioSerial.setTimeout(1);
  radioSerial.begin(9600);
  Serial.begin(9600);
  
  Stream &mySerial = (Stream &)radioSerial;
  E220 radioModule(&mySerial, M0, M1, AUX);
  Serial.println("Initializing...");
  while(!radioModule.init()){
        delay(5000);
  }
  radioModule.setChannel(RADIO_CHANNEL, true);
  radioModule.setAddress(1234,true);
  Serial.print("Set Address: ");
  Serial.println(radioModule.getAddress());

  radioModule.setPower(Power_21,false);
  Serial.print("Set power: ");
  Serial.println(radioModule.getPower());
  
  Serial.print("Set Channel: ");
  Serial.println(radioModule.getChannel());
  
}

void loop() {
  // put your main code here, to run repeatedly:
    if(radioSerial.available()){
        Serial.print(radioSerial.readString());
    }
    if(Serial.available()){
      radioSerial.print(" " + Serial.readString());
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
  radioSerial.println(strcat("03 This is ", FCC_CALL_SIGN));
}
