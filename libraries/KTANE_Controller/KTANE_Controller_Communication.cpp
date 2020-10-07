/*
  KTANE_Controller_Communication.h - Helper class for getting moudule data from the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#include "Arduino.h"
#include <Wire.h>
#include <KTANE_Controller_Communication.h>

//Helper functions

void KTANE_Controller_Communication::send(int module, byte value){
  Wire.beginTransmission(module);
  Wire.write(value);
  Wire.endTransmission();
}

String KTANE_Controller_Communication::receiveUnknownSize(int module){
  Wire.requestFrom(module, 1);
  byte size = Wire.read();
  byte fixedSize = 32;
  if (size<32){
    fixedSize = size;
  }
  char result[fixedSize];
  Wire.requestFrom(module, fixedSize);
  for (int i=0; i<fixedSize; i++){
    if(Wire.available()){
      result[i] = Wire.read();
    }else{
      result[i] = '?';
    }
  }
  if (size>32){
    return String(result)+ "...";
  }else{
    return String(result);
  }
}

// Functions
void KTANE_Controller_Communication::begin(){
  Wire.begin(); // join i2c bus (address optional for master)
}

void KTANE_Controller_Communication::arm(int module){
  send(module, 20);
}

void KTANE_Controller_Communication::reset(int module){
  send(module, 21);
}

void KTANE_Controller_Communication::explode(int module){
  send(module, 22);
}

void KTANE_Controller_Communication::win(int module){
  send(module, 23);
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
  send(module, 4);
  Wire.requestFrom(module, 1);
  return Wire.read();
}

String KTANE_Controller_Communication::receiveFullConfig(int module){
  send(module, 1);
  return receiveUnknownSize(module);
}

String KTANE_Controller_Communication::receiveErrorMessage(int module){
  send(module, 2);
  return receiveUnknownSize(module);
}

String KTANE_Controller_Communication::receiveDebugMessage(int module){
  send(module, 3);
  return receiveUnknownSize(module);
}

String KTANE_Controller_Communication::receiveModuleName(int module){
  send(module, 5);
  return receiveUnknownSize(module);
}

String KTANE_Controller_Communication::receiveModuleVersion(int module){
  int version[3];
  send(module, 6);
  Wire.requestFrom(module, 3);
  for (int i=0; i<3; i++){
    if (Wire.available()){
      version[i] = Wire.read();
    }else{
      version[i] = 0;
    }
  }
  return String(version[0]) + "." + String(version[1]) + "." + String(version[2]);
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
KTANE_Controller_Communication ktaneCC = KTANE_Controller_Communication();
