/*
  KTANE_Controller.h - Library for storing multiple module's data and running commands to all of them on the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Controller_h
#define KTANE_Controller_h

#include "Arduino.h"
#include "KTANE_Controller_Communication.h"
#include "KTANE_Controller_Module.h"
class KTANE_Controller {
  private:
    int modulesLength;

    // GameState
    int batteries;
    bool parallelPort;
    bool serialOdd;
    bool indicators[11];
    unsigned long explosionTime;

    KTANE_Controller_Module modules[];
  public:
    // gameState
    bool allArmed();
    bool allDisarmed();
    bool allValidConfig();
    bool allInvalidConfig();
    int strikes();
    String bombFullConfig();

    bool moduleArmed(int);
    int moduleStrikes(int);
    String moduleErrorMessage(int);
    String moduleDebugMessage(int);
    String moduleName(int);
    String moduleVersion(int);
    String moduleFullConfig(int);


    //get and set
    int getBatteries();
    bool getParallelPort();
    bool getSerialOdd();
    bool getIndicator(int);
    bool getExplosionTime();
    bool getTimeTillExplosion();


    void setBatteries(int);
    void SetParallelPort(bool);
    void setSerialOdd(bool);
    void setIndicator(int, bool);
    void setIndicator(bool[11]);
    void setExplosionTime(unsigned long);
    void setTimeTillExplosion(unsigned long);


    
    

    // Actions
    void arm();
    void reset();
    void explode();
    void win();

    // needed Info
    void sendNeededInfo();
    void forceSendNeededInfo();
    


};

#endif
