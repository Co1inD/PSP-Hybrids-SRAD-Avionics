#ifdef Test_Board
#ifndef M_PI
#include <math.h>
#endif



float velocity[] = {0, 0, 0};
float acceleration[] = {0, 0, 0};
float position[] = {0, 0, 0};
float orientation[] = {0, 0};
float baro = 0;

float* getVelocity() {
  return velocity;
}

float* getAcceleration() {
  return acceleration;
}

float* getPosition(){
  return position;
}
// return altitude in meters with respect to any static reference point
float getBaro(){
  return baro;
}

float* getOrientation(){
  return orientation;
}

void launchChute(int number) {
  queueRecord("booom!");
}

void doSensorReady(){
  // some stuff
}

//Produces a buzzer pattern depending on what mode is passed in. 
void buzzer(int mode){
 // no stuff
}
void update() {
  position[0] = sin(millis()/2000.0);
  position[1] = cos(millis()/2000.0);
  //velocity[0] = cos(millis()/2000.0);
  velocity[1] = 250 + 250 * -sin(millis()/2000.0);
  //acceleration[0] = -sin(millis()/2000.0);
  acceleration[1] = 50 + 50 * - cos(millis()/2000.0);
  // call the filter
  
  // some stuff
}


#endif