/*
  KTANE_Controller_Communication.h - Library for storing module data on the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Controller_Communication_h
#define KTANE_Controller_Communication_h

#include "Arduino.h"
#include <Wire.h>

class KTANE_Controller_Communication {
  private:
  public:
    // Get Functions
    byte receiveStatus(int);
    int receiveStrikes(int);
    String receiveFullConfig(int);
    String receiveErrorMessage(int);
    String receiveDebugMessage(int);
    String receiveModuleName(int);
    int[] receiveModuleVersion(int);
    
    // Set Functions
    void sendBatteries(int, int);
    void sendParallelPort(int, bool);
    void sendSerialOdd(int, bool);
    void sendIndicators(int, bool[11]);
    void sendTimeTillExplosion(int, unsigned long);
    // send all?
    // data preferences?
    
    // Actions
    void arm(int);
    void reset(int);
    void explode(int);
    void win(int);


};

#endif
