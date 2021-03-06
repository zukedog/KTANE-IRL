/*
  KTANE_Slave.cpp - Library for slave modules that deals with game states and\
  handles communication with the master module over I2C (Wire)\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Slave_h
#define KTANE_Slave_h

#include "Arduino.h"

class KTANE_Slave {

  private:
    //user set callbacks
    static bool (*user_onArm)();
    static void (*user_onReset)();
    static void (*user_onExplode)();
    static void (*user_onWin)();

    static bool _gameMode;
    static bool _validConfig;
    static bool _armed;

    static bool newFullConfig;
    static bool newErrorMessage;
    static bool newDebugMessage;
    static bool needGameState;
    static bool hasStrike;

    static String fullConfig;
    static String errorMessage;
    static String debugMessage;

    static String moduleName;
    static byte moduleVersion[3];

    static byte strikes;

    // wire
    static void receiveEvent(int);
    static void requestEvent();
    static void writeDataOrLength(String data, bool sendData);

    static bool sentSize;
    static byte masterRequestValue;

    static void arm();
    static void reset();
    static void explode();
    static void win();

    static void sendStatusToMaster();
    static void sendConfigToMaster();
    static void sendDebugToMaster();
    static void sendNameToMaster();
    static void sendErrorToMaster();
    static void sendStrikesToMaster();
    static void sendVersionToMaster();

  public:

    //Test
    void printVars();

    //functions
    void begin();
    void begin(String);
    void begin(String, byte);
    void begin(String, byte, byte, byte);

    void debug(String);
    void debugln(String);

    //gets
    bool gameMode();
    bool configMode();
    bool armed();
    bool configValid();

    //sets
    void disarm();
    void strike();
    void strike(int);
    void setValidConfig(bool);
    void setFullConfig(String);

    //callbacks
    void onArm( bool (*)() );
    void onReset( void (*)() );
    void onExplode( void (*)() );
    void onWin( void (*)() );
};

#endif
