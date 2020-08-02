#include <KTANE_Module.h>
KTANE_Module KTANE = KTANE_Module();

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  
  Serial.begin(9600);
  KTANE.begin("DumbArse", 0, 1, 0);
  KTANE.onArm(onArm); //callback function for when the bomb arms, see onArm() bellow
  KTANE.onReset(onReset); //callback function for when the bomb resets, see onReset() bellow
  KTANE.onExplode(onExplode); //callback function for when the bomb Explodes, see onExplode() bellow
  KTANE.onWin(onWin); //callback function for when the bomb wins, see onWin() bellow
  
}

void loop() {
  
  //if we are in gamemode run the gameLoop() function.
  //if we are in config mode run the configLoop() function
  //these are seperated into seperate functions instead of nesting them here for clarity.

  if (KTANE.gameMode() == true) {
    //gameLoop();
  }

  if (KTANE.configMode() == true) { //could also replace this with (KTANE.gameMode() == false) as the module is either in config mode or game mode never both
    //configLoop();
  }

  checkSerialDebug();
  
  
}


void checkSerialDebug(){
  if (stringComplete) {

    if (inputString == "disarm\n") {
      KTANE.disarm();
    }
    if (inputString == "strike\n"){
      KTANE.strike();
      Serial.println("STRIKE");
    }
    if (inputString == "invalidConfig\n"){
      KTANE.setValidConfig(false);
    }
    if (inputString == "validConfig\n"){
      KTANE.setValidConfig(true);
      Serial.println("validConfigSet");
    }
    if (inputString.substring(0, 6) == "debug:"){
      KTANE.debug(inputString.substring(6));
    }
    if (inputString.substring(0, 6) == "error:"){
      Serial.println("ERROR no ERROR function yet");
      //KTANE.error(inputString.substring(6));
    }
    if (inputString.substring(0, 7) == "config:"){
      KTANE.setFullConfig(inputString.substring(7));
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
}



void gameLoop() {
  if (KTANE.armed() == false) {
    return; //if this module isn't armed then we don't need to do anything other than wait for a callback function to be called
    //callback functions are interupt based so we can just do nothing by returning.
  }


  //The check for being armed has been made above so we know if we got to this point we're armed
  //do whatever we want to do to during the game to check if the player has put the module into a state
  //safe for disarming.

  if (/*player put module in correct state for disarming*/ false) {
    KTANE.disarm(); //disarm the module
  }

  //also check if the player has done something to trigger a strike

  if (/*player pressed wrong thing*/ false) {
    KTANE.strike(1); //increase strikes by 1 (might make this not need the int)
  }

}

void configLoop() {
  // if this is called the bomb is in config mode which means we want to know if it is in a valid config
  // we need to set this by using validConfig(bool isValid)

  if (/*whatever requrements you need to be fufilled to have a valid config*/ true) {
    //KTANE.setValidConfig(true);
  }
  else {
    KTANE.setValidConfig(false);
  }

}

bool onArm() {
  /* This function if a callback is added with onArm() will run whenever the module is asked to
     arm itself by the Master. It should return true if the arm was successfull or false if the arm failed.
     The library will deal with changing states at this point.
  */
  
  //configLoop(); //don't need as should be looping but might as well update the config justin case

  if (KTANE.configValid() == true) {
    Serial.println("ARMIFICATED");
    return true;
  } else {
    Serial.println("Failed to ARM, config invalid. Try legging next time?");
    return false;
  }

}

void onReset() {
  /* This function if a callback is added with onReset() should be used to run all code to reset the module to its initial state.
     The Master will issue this command when the game has been finished and needs to be reset.
  */
  Serial.println("YOUVE BEEN RESET");
}

void onExplode() {
  /* This function if a callback is added with onExplode() will be called when the master tells this module the bomb
      has exploded. This should be used if this module has any special effects that need to be activated if the game is lost.
  */
  Serial.println("YOUVE BEEN EXPLODIFICATED");
}

void onWin() {
  /* This function if a callback is added with onWin() will be called when the master tells this module the bomb
      has been safley diffused. This should be used if this module has any special effects that need to be activated if the game is won.
  */
  Serial.println("YOUVE WON. WHOOPDYDOOOOO.");
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
