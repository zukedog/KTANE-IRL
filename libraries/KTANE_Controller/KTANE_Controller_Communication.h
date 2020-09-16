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
    byte getStatus(int);
    int getStrikes(int);
    String getFullConfig(int);
    String getErrorMessage(int);
    String getDebugMessage(int);
    String getModuleName(int);
    int[] getModuleVersion(int);
    
    // Set Functions
    void setBatteries(int, int);
    void setParallelPort(int, bool);
    void setSerialOdd(int, bool);
    void setIndicators(int, bool[11]);
    void setTimeTillExplosion(int, unsigned long);


};

#endif
