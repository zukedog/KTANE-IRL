/*
  KTANE_Slave.cpp - Library for slave modules that deals with game states and\
  handles communication with the master module over I2C (Wire)\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/

#include "KTANE_Slave.h"
#include <Wire.h>
bool (*KTANE_Slave::user_onArm)(void);
void (*KTANE_Slave::user_onReset)(void);
void (*KTANE_Slave::user_onExplode)(void);
void (*KTANE_Slave::user_onWin)(void);



// static definitions
bool KTANE_Slave::_gameMode = false;
bool KTANE_Slave::_validConfig = false;
bool KTANE_Slave::_armed;

bool KTANE_Slave::newFullConfig = false;
bool KTANE_Slave::newErrorMessage = false;
bool KTANE_Slave::newDebugMessage = false;
bool KTANE_Slave::needGameState = false;
bool KTANE_Slave::hasStrike = false;

String KTANE_Slave::fullConfig = "NoConfigSet";
String KTANE_Slave::errorMessage = "";
String KTANE_Slave::debugMessage = "";

String KTANE_Slave::moduleName = "NONE";
byte KTANE_Slave::moduleVersion[3] = {0, 0, 0};

byte KTANE_Slave::strikes = 0;

// Wire Stuff

bool KTANE_Slave::sentSize = false;


byte KTANE_Slave::masterRequestValue;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void KTANE_Slave::receiveEvent(int howMany) {

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


void KTANE_Slave::requestEvent() {
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

void KTANE_Slave::sendNameToMaster() {
  writeDataOrLength(moduleName, sentSize);
  sentSize = ! sentSize;
}

void KTANE_Slave::sendErrorToMaster() {
  writeDataOrLength(errorMessage, sentSize);
  sentSize = ! sentSize;
  newErrorMessage = false;
}

void KTANE_Slave::sendStrikesToMaster() {
  Wire.write(strikes);
  hasStrike = false;
}

void KTANE_Slave::sendVersionToMaster() {
  for (int i = 0; i < 3; i++) {
    Wire.write(moduleVersion[i]);
  }
}

void KTANE_Slave::sendDebugToMaster() {
  writeDataOrLength(debugMessage, sentSize);
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

void KTANE_Slave::sendConfigToMaster() {
  writeDataOrLength(fullConfig, sentSize);
  sentSize = ! sentSize;
  newFullConfig = false;
}



void KTANE_Slave::writeDataOrLength(String data, bool sendData) {
  if (sendData) {
    Wire.write(data.c_str());
  }
  else {
    Wire.write(data.length());
  }
}





//not wire Stuff




void KTANE_Slave::begin(String moduleName, byte versionBreaking, byte versionFeature, byte versionBugfix) {
  this->moduleName = moduleName; //because of pointers and stuff you need to use the "->" instead of a "."
  moduleVersion[0] = versionBreaking;
  moduleVersion[1] = versionFeature;
  moduleVersion[2] = versionBugfix;

  //Wire stuff
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event

}

void KTANE_Slave::begin(String moduleName, byte versionBreaking) {
  begin(moduleName, versionBreaking, 0, 0);
}

void KTANE_Slave::begin(String moduleName) {
  begin(moduleName, 0, 0 , 0);
}

void KTANE_Slave::begin() {
  begin("DefaultName", 0, 0, 0);
}


void KTANE_Slave::debug(String message) {
  debugMessage.concat(message);
  newDebugMessage = true;
}

void KTANE_Slave::debugln(String message) {
  debug(message + "\n");
  newDebugMessage = true;
}

//gets
bool KTANE_Slave::gameMode() {
  return _gameMode;
}
bool KTANE_Slave::configMode() {
  return !_gameMode;
}
bool KTANE_Slave::armed() {
  return _armed;
}
bool KTANE_Slave::configValid() {
  return _validConfig;
}

//sets
void KTANE_Slave::disarm() {
  _armed = false;
}
void KTANE_Slave::strike(int strikes) {
  this->strikes += strikes;
  hasStrike = true;
}
void KTANE_Slave::strike() {
  strike(1);
}
void KTANE_Slave::setValidConfig(bool validity) {
  _validConfig = validity;
}
void KTANE_Slave::setFullConfig(String config) {
  fullConfig = config;
}

void KTANE_Slave::onArm( bool (*function)() ) {
  user_onArm = function;
}

void KTANE_Slave::onReset( void (*function)() ) {
  user_onReset = function;
}

void KTANE_Slave::onExplode( void (*function)() ) {
  user_onExplode = function;
}

void KTANE_Slave::onWin( void (*function)() ) {
  user_onWin = function;
}


void KTANE_Slave::arm() {
  if (!user_onArm) {
    return; //return if the user forgot to add an onArm function
  }
  if (user_onArm()) { // run users on arm function
    _armed = true;
    _gameMode = true; // if success arm the module
  }

}
void KTANE_Slave::reset() {
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
void KTANE_Slave::explode() {
  if (user_onExplode) {
    user_onExplode();
  }
}
void KTANE_Slave::win() {
  if (user_onWin) {
    user_onWin();
  }
}

void KTANE_Slave::sendStatusToMaster() {
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
