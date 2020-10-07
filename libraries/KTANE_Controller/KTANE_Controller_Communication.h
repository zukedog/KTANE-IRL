/*
  KTANE_Controller_Communication.h - Helper class for getting moudule data from the controller module\
  for JustinCase and Zukedog2000's Keep Talking and Nobody Explodes project.
  Created by Zukedog2000, June 2020.
*/
#ifndef KTANE_Controller_Communication_h
#define KTANE_Controller_Communication_h

#include "Arduino.h"
#include <Wire.h>

class KTANE_Controller_Communication {
  private:
    static void send(int, byte);
    static String receiveUnknownSize(int);
  public:
    // Functions
    static void begin();
    static void arm(int);
    static void reset(int);
    static void explode(int);
    static void win(int);

    // Get Functions
    static byte receiveNeededInfo(int);

    static byte receiveStatus(int);
    static int receiveStrikes(int);
    static String receiveFullConfig(int);
    static String receiveErrorMessage(int);
    static String receiveDebugMessage(int);
    static String receiveModuleName(int);
    static String receiveModuleVersion(int);
    
    // Set Functions
    static void sendBatteries(int, int);
    static void sendParallelPort(int, bool);
    static void sendSerialOdd(int, bool);
    static void sendIndicators(int, bool[11]);
    static void sendTimeTillExplosion(int, unsigned long);
    // send all?
    // data preferences?
    

};
extern KTANE_Controller_Communication ktaneCC;

#endif
