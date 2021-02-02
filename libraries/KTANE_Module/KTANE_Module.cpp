/*
  KTANE_Module.cpp - Library for slave modules that deals with game states and\
  handles communication with the master module over I2C (Wire)\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/

#include "KTANE_Module.h"
#include <Wire.h>
#define KTANE_INDICATOR_SND 0
#define KTANE_INDICATOR_CLR 1
#define KTANE_INDICATOR_CAR 2
#define KTANE_INDICATOR_IND 3
#define KTANE_INDICATOR_FRQ 4
#define KTANE_INDICATOR_SIG 5
#define KTANE_INDICATOR_NSA 6
#define KTANE_INDICATOR_MSA 7
#define KTANE_INDICATOR_TRN 8
#define KTANE_INDICATOR_BOB 9
#define KTANE_INDICATOR_FRK 10


bool (*KTANE_Module::user_onArm)(void);
void (*KTANE_Module::user_onReset)(void);
void (*KTANE_Module::user_onExplode)(void);
void (*KTANE_Module::user_onWin)(void);



// static definitions
bool KTANE_Module::_gameMode = false;
bool KTANE_Module::_validConfig = false;
bool KTANE_Module::_armed;

int KTANE_Module::_batteries = 0;
bool KTANE_Module::parallelPort = false; // Sorry
bool KTANE_Module::_serialOdd = false;
bool KTANE_Module::_serialVowel = false;
bool KTANE_Module::indicators[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long KTANE_Module::explosionTime = 3600000;


bool KTANE_Module::newFullConfig = false;
bool KTANE_Module::newErrorMessage = false;
bool KTANE_Module::newDebugMessage = false;
bool KTANE_Module::needGameState = false;
bool KTANE_Module::hasStrike = false;

String KTANE_Module::fullConfig = "NoConfigSet";
String KTANE_Module::errorMessage = "";
String KTANE_Module::debugMessage = "";

String KTANE_Module::moduleName = "UNNAMED";
byte KTANE_Module::moduleVersion[3] = {0, 0, 0};

byte KTANE_Module::strikes = 0;

// Wire i2c Stuff

bool KTANE_Module::sentSize = false;


byte KTANE_Module::masterRequestValue;


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void KTANE_Module::receiveEvent(int howMany) {

  while (Wire.available()) {
    int received = Wire.read();
    if (received < 10) {
      masterRequestValue = received; // set the request variable so we know what data to send back
    }
    switch (received) {
      case 20:
        arm();
        break;

      case 21:
        reset();
        break;

      case 22:
        explode();
        break;

      case 23:
        win();
        break;

    }


  }
}


void KTANE_Module::requestEvent() {
  switch (masterRequestValue) {
    case 0:
      sendStatusToMaster();
      break;

    case 1:
      sendConfigToMaster();
      break;

    case 2:
      sendErrorToMaster();
      break;

    case 3:
      sendDebugToMaster();
      break;

    case 4:
      sendStrikesToMaster();
      break;

    case 5:
      sendNameToMaster();
      break;

    case 6:
      sendVersionToMaster();
      break;
  }
}



void KTANE_Module::sendNameToMaster() {
  writeDataOrLength(moduleName, sentSize);
  sentSize = ! sentSize;
}

void KTANE_Module::sendErrorToMaster() {
  writeDataOrLength(errorMessage, sentSize);
  sentSize = ! sentSize;
  newErrorMessage = false;
}

void KTANE_Module::sendStrikesToMaster() {
  Wire.write(strikes);
  strikes = 0;
  hasStrike = false;
}

void KTANE_Module::sendVersionToMaster() {
  for (int i = 0; i < 3; i++) {
    Wire.write(moduleVersion[i]);
  }
}

void KTANE_Module::sendDebugToMaster() {
  if (sentSize) {
    Wire.write(debugMessage.c_str());
    debugMessage = "";
  }
  else {
    Wire.write(debugMessage.length());
  }
  sentSize = ! sentSize;
  newDebugMessage = false;

}

void KTANE_Module::sendConfigToMaster() {
  writeDataOrLength(fullConfig, sentSize);
  sentSize = ! sentSize;
  newFullConfig = false;
}



void KTANE_Module::writeDataOrLength(String data, bool sendData) {
  if (sendData) {
    Wire.write(data.c_str());
  }
  else {
    Wire.write(data.length());
  }
}





//not wire Stuff




void KTANE_Module::begin(String moduleName, byte versionBreaking, byte versionFeature, byte versionBugfix) {
  this->moduleName = moduleName; //because of pointers and stuff you need to use the "->" instead of a "."
  moduleVersion[0] = versionBreaking;
  moduleVersion[1] = versionFeature;
  moduleVersion[2] = versionBugfix;

  //Wire stuff
  int address = (analogRead(6)+47)/93;
  bool different = false;
  for (int i=0; i <= 40; i++){
    if(address != (analogRead(6)+47)/93){
      different = true;
    }
  }
  if(different){
    Wire.begin(8);                // join i2c bus with address #8
  }else{
    Wire.begin(address+10);
  }
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event

}

void KTANE_Module::begin(String moduleName, byte versionBreaking) {
  begin(moduleName, versionBreaking, 0, 0);
}

void KTANE_Module::begin(String moduleName) {
  begin(moduleName, 0, 0 , 0);
}

void KTANE_Module::begin() {
  begin("DefaultName", 0, 0, 0);
}


void KTANE_Module::debug(String message) {
  debugMessage.concat(message);
  newDebugMessage = true;
}

void KTANE_Module::debugln(String message) {
  debug(message + "\n");
  newDebugMessage = true;
}

//gets
bool KTANE_Module::gameMode() {
  return _gameMode;
}
bool KTANE_Module::configMode() {
  return !_gameMode;
}
bool KTANE_Module::armed() {
  return _armed;
}
bool KTANE_Module::configValid() {
  return _validConfig;
}

int KTANE_Module::batteries(){
  return _batteries;
}

bool KTANE_Module::port(int index){
  return parallelPort;
}

bool KTANE_Module::port(){
  port(1);
}

bool KTANE_Module::serialOdd(){
  return _serialOdd;
}

bool KTANE_Module::serialEven(){
  return !serialOdd();
}

bool KTANE_Module::serialVowel(){
  return _serialVowel;
}

bool KTANE_Module::indicator(int index){
  return indicators[index]; 
}


unsigned long KTANE_Module::timeTillDetonation(){
  return explosionTime - millis(); 
}

int KTANE_Module::getStrikes(){
  return 0;

}

//sets
void KTANE_Module::disarm() {
  _armed = false;
}
void KTANE_Module::strike(int strikes) {
  this->strikes += strikes;
  hasStrike = true;
}
void KTANE_Module::strike() {
  strike(1);
}
void KTANE_Module::setValidConfig(bool validity) {
  _validConfig = validity;
}
void KTANE_Module::setFullConfig(String config) {
  fullConfig = config;
}

void KTANE_Module::onArm( bool (*function)() ) {
  user_onArm = function;
}

void KTANE_Module::onReset( void (*function)() ) {
  user_onReset = function;
}

void KTANE_Module::onExplode( void (*function)() ) {
  user_onExplode = function;
}

void KTANE_Module::onWin( void (*function)() ) {
  user_onWin = function;
}


void KTANE_Module::arm() {
  if (!user_onArm) {
    return; //return if the user forgot to add an onArm function
  }
  if (user_onArm()) { // run users on arm function
    _armed = true;
    _gameMode = true; // if success arm the module
  }

}
void KTANE_Module::reset() {
  _armed = false;
  _gameMode = false;
  _validConfig = false;
  newDebugMessage = false;
  newErrorMessage = false;
  debugMessage = "";
  errorMessage = "";
  hasStrike = false;
  strikes = 0;
  // put more reset stuff here in future
  if (user_onReset) {
    user_onReset();
  }

}
void KTANE_Module::explode() {
  if (user_onExplode) {
    user_onExplode();
  }
}
void KTANE_Module::win() {
  if (user_onWin) {
    user_onWin();
  }
}

void KTANE_Module::sendStatusToMaster() {
  byte statusByte = 0;
  if (_gameMode) {
    statusByte += B10000000;
    if (_armed) {
      statusByte += B01000000;
    }
  } else {
    if (_validConfig) {
      statusByte += B01000000;
    }
  }
  if (newFullConfig) {
    statusByte += B00100000;
  }
  if (newErrorMessage) {
    statusByte += B00010000;
  }
  if (newDebugMessage) {
    statusByte += B00001000;
  }
  if (hasStrike) {
    statusByte += B00000100;
  }
  if (needGameState) {
    statusByte += B00000010;
  }
  //statusByte += B00000001;

  Wire.write(statusByte); //status Just send something random for the moment while testing
}
