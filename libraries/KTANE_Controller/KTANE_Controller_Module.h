/*
  KTANE_Controller_Module.h - Data structure for storing module data on the controller module\
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
    bool responding; // Used to detirmine if the module responded to the last message
    bool initialised; // Used to detirmine if the module has been initialised since it last stopped responding

    // Module State
    byte status;
    int strikes;
    String fullConfig;
    String errorMessage;
    String debugMessage;
    String moduleName;
    int moduleVersion[3];

    byte neededInfo;
    // Document this later (Bits represent want for TBPS)

  public:
    // Get Functions
    bool getGameMode();
    bool getArmed();
    bool getValidConfig();
    String getConfig();
    String getErrorMessage();
    String getDebugMessage();
    int getStrikes();

    bool getWantBatteries();
    bool getWantParallelPort();
    bool getWantSerialOdd();
    bool getWantIndicators();
    bool getWantTimeTillExplosion();
    byte getNeededInfo();

    int getAddress();

    // Update Functions
    void updateStatus();
    void updateStateAndStatus();
    void updateEverything();

    // Action Functions
    void arm();
    void reset();
    void explode();
    void win();

    
    
};

#endif
