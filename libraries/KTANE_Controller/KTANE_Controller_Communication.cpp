/*
  KTANE_Controller_Communication.h - Helper class for getting moudule data from the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#include "Arduino.h"
#include <Wire.h>
#include <KTANE_Controller_Communication.h>

// Functions
void KTANE_Controller_Communication::begin(){
  Wire.begin(); // join i2c bus (address optional for master)
}

void KTANE_Controller_Communication::arm(int module){
}

void KTANE_Controller_Communication::reset(int module){
}

void KTANE_Controller_Communication::explode(int module){
}

void KTANE_Controller_Communication::win(int module){
}

// Get Functions
byte KTANE_Controller_Communication::receiveNeededInfo(int module){
  // needed info is the game state information required by the module\
  // this will eventually be use to only send the information the module needs.
}

byte KTANE_Controller_Communication::receiveStatus(int module){
  Wire.beginTransmission(module);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(module, 1);
  return Wire.read();
}

int KTANE_Controller_Communication::receiveStrikes(int module){
  return 0;
}

String KTANE_Controller_Communication::receiveFullConfig(int module){
  return "";
}

String KTANE_Controller_Communication::receiveErrorMessage(int module){
  return "";
}

String KTANE_Controller_Communication::receiveDebugMessage(int module){
  return "";
}

String KTANE_Controller_Communication::receiveModuleName(int module){
  return "";
}

String KTANE_Controller_Communication::receiveModuleVersion(int module){
  return "";
}


// Set Functions
void KTANE_Controller_Communication::sendBatteries(int module, int batteries){
}

void KTANE_Controller_Communication::sendParallelPort(int module, bool port){
}

void KTANE_Controller_Communication::sendSerialOdd(int module, bool odd){
}

void KTANE_Controller_Communication::sendIndicators(int module, bool indicators[11]){
}

void KTANE_Controller_Communication::sendTimeTillExplosion(int module, unsigned long remainingTime){
}

// send all?
// data preferences?


