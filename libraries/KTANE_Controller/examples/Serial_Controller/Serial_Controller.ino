#include <Wire.h>
#include <KTANE_Controller_Communication.h>
#include <KTANE_Controller_Module.h>
#define moduleCount 12
bool connection[moduleCount];
//KTANE_Controller_Module module;
KTANE_Controller_Module modules[moduleCount];
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
const int address = 8;
String brk = "--------------------------------------------------------------";
void setup() {
  Serial.begin(2000000);  // start serial for output
  ktaneCC.begin();
  //module.begin(address);
  for (int i = 0; i < moduleCount; i++) {
    connection[i] = false;
    modules[i].begin(i + 10);
  }



  // reserve 200 bytes for the inputString:
  inputString.reserve(10);
}


void printStatus(int moduleNum) {
  Serial.println("MODULE " +  String(moduleNum) + " STATUS:");
  Serial.print("GM: ");
  Serial.print(modules[moduleNum].getGameMode());
  Serial.print(", VC/A: ");
  Serial.print(modules[moduleNum].getValidConfig());
  Serial.print(", STR: ");
  Serial.println(modules[moduleNum].getStrikes());
  Serial.println(brk);

}


void loop() {
  //Serial.println("loop");
  //delay(1000);
  for (int i = 0; i < moduleCount; i++) {
    modules[i].updateStateAndStatus();

    if (modules[i].getResponding() != connection[i]) {
      connection[i] = modules[i].getResponding();
      if (connection[i]) {
        
        Serial.print("Connected to Module ");
        Serial.println(String(i));
        Serial.println(brk);
        printStatus(i);
      } else {
        Serial.print("Disconected from Module ");
        Serial.println(String(i));
        Serial.println(brk);

      }
    }

    if (modules[i].newStatusAvailable() || modules[i].newStrikesAvailable()) {
      printStatus(i);
    }

    if (modules[i].newDebugAvailable()) {
      Serial.print("DEBUG FROM " + String(i) + ": ");
      Serial.println(modules[i].getDebugMessage());
      Serial.println(brk);
    }
  }



  if (stringComplete) {
    if (inputString == "arm\n") {
      for (int i = 0; i < moduleCount; i++) {
        modules[i].arm();
      }
    }

    if (inputString == "reset\n") {
      for (int i = 0; i < moduleCount; i++) {
        modules[i].reset();
      }
    }

    if (inputString == "explode\n") {
      for (int i = 0; i < moduleCount; i++) {
        modules[i].explode();
      }
    }

    if (inputString == "win\n") {
      for (int i = 0; i < moduleCount; i++) {
        modules[i].win();
      }
    }

    if (inputString == "getStatus\n") {
      Serial.println(ktaneCC.receiveStatus(address), BIN);
    }

    if (inputString == "getConfig\n") {
      Serial.println(ktaneCC.receiveFullConfig(address));
    }

    if (inputString == "getError\n") {
      Serial.println(ktaneCC.receiveErrorMessage(address));
    }

    if (inputString == "getDebug\n") {
      Serial.println(ktaneCC.receiveDebugMessage(address));
    }

    if (inputString == "getStrikes\n") {
      Serial.println(ktaneCC.receiveStrikes(address));
    }

    if (inputString == "getName\n") {
      Serial.println(ktaneCC.receiveModuleName(address));
    }

    if (inputString == "getVersion\n") {
      Serial.println(ktaneCC.receiveModuleVersion(address));
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }



}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
