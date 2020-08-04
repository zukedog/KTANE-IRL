/*
  KTANE_Controller.h - Library for storing module data on the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Controller_h
#define KTANE_Controller_h

#include "Arduino.h"
//#include <Wire.h>

class KTANE_Controller {
  private:
    bool gameMode;
    bool validConfig;
    bool armed;

    bool newFullConfig;
    bool newErrorMessage;
    bool newDebugMessage;
    bool needTime;
    bool hasStrike;

    String fullConfig;
    String errorMessage;
    String debugMessage;

    String moduleName;
    byte moduleVersion[3];

    byte strikes;

  public:
    void begin();

    // request info from slave
    void getStatus();
    void getConfig();
    void getError();
    void getDebug();
    void getStrike();
    void getName();
    void getVersion();

    // Give slave command
    void arm();
    void reset();
    void explode();
    void win();


    // Send slave data
    void sendTime();
    void sendTBPS();
    void sendDataPrefs();

    // want Debug
    // want Error
    // want config
    // want name & version


};

#endif
