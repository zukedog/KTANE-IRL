#include <Wire.h>
#include <KTANE_Controller_Communication.h>
#include <KTANE_Controller_Module.h>

KTANE_Controller_Module module;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(2000000);  // start serial for output
  ktaneCC.begin();
  module.begin(8);

  // reserve 200 bytes for the inputString:
  inputString.reserve(10);
}



void loop() {
  //Serial.println("loop");
  //delay(1000);
  module.updateStateAndStatus();

  if (module.newStatusAvailable() || module.newStrikesAvailable()) {
    Serial.println("NEW STATUS RECEIVED");
    Serial.print("GM: ");
    Serial.print(module.getGameMode());
    Serial.print(", VC/A: ");
    Serial.print(module.getValidConfig());
    Serial.print(", STR: ");
    Serial.println(module.getStrikes());
    Serial.println("--------------------------------------------------------------");


  }

  if (module.newDebugAvailable()) {
    Serial.print("DEBUG: ");
    Serial.println(module.getDebugMessage());
    Serial.println("--------------------------------------------------------------");
  }



  if (stringComplete) {
    if (inputString == "arm\n") {
      module.arm();
    }

    if (inputString == "reset\n") {
      module.reset();
    }

    if (inputString == "explode\n") {
      module.explode();
    }

    if (inputString == "win\n") {
      module.win();
    }

    if (inputString == "getStatus\n") {
      Serial.println(ktaneCC.receiveStatus(8), BIN);
    }

    if (inputString == "getConfig\n") {
      Serial.println(ktaneCC.receiveFullConfig(8));
    }

    if (inputString == "getError\n") {
      Serial.println(ktaneCC.receiveErrorMessage(8));
    }

    if (inputString == "getDebug\n") {
      Serial.println(ktaneCC.receiveDebugMessage(8));
    }

    if (inputString == "getStrikes\n") {
      Serial.println(ktaneCC.receiveStrikes(8));
    }

    if (inputString == "getName\n") {
      Serial.println(ktaneCC.receiveModuleName(8));
    }

    if (inputString == "getVersion\n") {
      Serial.println(ktaneCC.receiveModuleVersion(8));
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
