#include <Wire.h>
#include <KTANE_Controller_Communication.h>
#include <KTANE_Controller_Module.h>

bool connection = false;
KTANE_Controller_Module module;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
const int address = 8;
void setup() {
  Serial.begin(2000000);  // start serial for output
  ktaneCC.begin();
  module.begin(address);

  // reserve 200 bytes for the inputString:
  inputString.reserve(10);
}


void printStatus(){
    Serial.println("MODULE STATUS:");
    Serial.print("GM: ");
    Serial.print(module.getGameMode());
    Serial.print(", VC/A: ");
    Serial.print(module.getValidConfig());
    Serial.print(", STR: ");
    Serial.println(module.getStrikes());
    Serial.println("--------------------------------------------------------------");

}


void loop() {
  //Serial.println("loop");
  //delay(1000);
  module.updateStateAndStatus();

  if(module.getResponding() != connection){
    connection = module.getResponding();
    if(connection){
      Serial.println("Connected to Module");
      Serial.println("--------------------------------------------------------------");
      printStatus();
    }else{
      Serial.println("Disconected from Module");
      Serial.println("--------------------------------------------------------------");

    }
  }

  if (module.newStatusAvailable() || module.newStrikesAvailable()) {
    printStatus();
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
