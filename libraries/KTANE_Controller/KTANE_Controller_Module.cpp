/*
  KTANE_Controller_Module.cpp - Data structure for storing module data on the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/

#include "Arduino.h"
#include "KTANE_Controller_Communication.h"
#include "KTANE_Controller_Module.h"
// Module Identity
// int address;
// bool responding; // Used to detirmine if the module responded to the last message
// bool initialised; // Used to detirmine if the module has been initialised since it last stopped responding

// Module State
// byte status;
// int strikes;
// String fullConfig;
// String errorMessage;
// String debugMessage;
// String moduleName;
// int moduleVersion[3];

// byte neededInfo;
// Document this later (Bits represent want for TBPS)



bool KTANE_Controller_Module::debugOnModule(){
  return status & B00001000;
}
bool KTANE_Controller_Module::errorOnModule(){
  return status & B00010000;
}
bool KTANE_Controller_Module::newConfigOnModule(){
  return status & B00100000;
}
bool KTANE_Controller_Module::newStrikesOnModule(){
  return status & B00000100;
}


void KTANE_Controller_Module::begin(int address){
  this->address = address;
  responding = false;
  initialised = false;
  status = B11111111;
  strikes = 0;
  fullConfig = "No Module Attatched";
  errorMessage = "";
  debugMessage = "";
  moduleName = "Disconnected";
  moduleVersion[0] = 0;
  moduleVersion[1] = 0;
  moduleVersion[2] = 0;
  neededInfo = 0;
  debugOnController = false;
  errorOnController = false;
  newConfigOnController = false;
  newStrikesOnController = false;
  newStatusOnController = false;
  initialised = false;
}

// Get Functions
bool KTANE_Controller_Module::getGameMode(){
  newStatusOnController = false;
  return status&B10000000;
}
bool KTANE_Controller_Module::getArmed(){
  newStatusOnController = false;
  return status & B01000000;
}
bool KTANE_Controller_Module::getValidConfig(){
  newStatusOnController = false;
  return status & B01000000;
}
String KTANE_Controller_Module::getConfig(){
  newConfigOnController = false;
  return fullConfig;
}
String KTANE_Controller_Module::getErrorMessage(){
  String returnValue = errorMessage;
  errorMessage == "";
  errorOnController = false;
  return returnValue;
}
String KTANE_Controller_Module::getDebugMessage(){
  String returnValue = debugMessage;
  debugMessage == "";
  debugOnController = false;
  return returnValue;
}
int KTANE_Controller_Module::getStrikes(){
  newStrikesOnController = false;
  return strikes;
}

bool KTANE_Controller_Module::newConfigAvailable(){
  return newConfigOnController;
}
bool KTANE_Controller_Module::newErrorAvailable(){
  return errorOnController;
}
bool KTANE_Controller_Module::newDebugAvailable(){
  return debugOnController;
}
bool KTANE_Controller_Module::newStrikesAvailable(){
  return newStrikesOnController;
}

bool KTANE_Controller_Module::newStatusAvailable(){
  return newStatusOnController;
}

bool KTANE_Controller_Module::getWantBatteries(){
  return true;
}
bool KTANE_Controller_Module::getWantParallelPort(){
  return true;
}
bool KTANE_Controller_Module::getWantSerialOdd(){
  return true;
}
bool KTANE_Controller_Module::getWantIndicators(){
  return true;
}
bool KTANE_Controller_Module::getWantTimeTillExplosion(){
  return true;
}
byte KTANE_Controller_Module::getNeededInfo(){
  return B11111111;
}

int KTANE_Controller_Module::getAddress(){
  return address;
}

// Update Functions
void KTANE_Controller_Module::updateStatus(){
  lastStatus = status;
  status = ktaneCC.receiveStatus(address);
  if ((status & B11000000) != (lastStatus & B11000000)){
    newStatusOnController = true;
  }
}
void KTANE_Controller_Module::updateStateAndStatus(){
  updateStatus();
  if (debugOnModule()){
    debugMessage += ktaneCC.receiveDebugMessage(address);
    debugOnController = true;
  }
  if (errorOnModule()){
    errorMessage += ktaneCC.receiveErrorMessage(address);
    errorOnController = true;
  }
  if (newConfigOnModule() || !initialised){
    fullConfig = ktaneCC.receiveFullConfig(address);
    newConfigOnController = true;
  }
  if (!initialised){
    strikes = 0;
  }
  if (newStrikesOnModule()) {
    strikes += ktaneCC.receiveStrikes(address);
    newStrikesOnController = true;
  }
  initialised = true;
}
void KTANE_Controller_Module::updateEverything(){
  updateStateAndStatus();
}

// Action Functions
void KTANE_Controller_Module::arm(){
  ktaneCC.arm(address);
}
void KTANE_Controller_Module::reset(){
  strikes = 0;
  newStrikesOnController = true;
  ktaneCC.reset(address);
}
void KTANE_Controller_Module::explode(){
  ktaneCC.explode(address);
}
void KTANE_Controller_Module::win(){
  ktaneCC.explode(address);
}
