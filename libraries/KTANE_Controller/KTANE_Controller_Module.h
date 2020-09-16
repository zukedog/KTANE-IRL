/*
  KTANE_Controller_Module.h - Library for storing module data on the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Controller_Module_h
#define KTANE_Controller_Module_h

#include "Arduino.h"
#include "KTANE_Controller_Communication.h"
class KTANE_Controller_Module {
  private:
    // Module Identity
    int address;

    // Module State
    byte status;
    int strikes;
    String fullConfig;
    String errorMessage;
    String debugMessage;
    String moduleName;
    int[3] moduleVersion;

  public:
    // Get Functions
    bool getGameMode();
    bool getArmed();
    bool getValidConfig();
    String getConfig();
    String getErrorMessage();
    String getDebugMessage();
    int getStrikes();

    // Update Functions
    void updateStatus();
    void updateStatus;
    
};

#endif
