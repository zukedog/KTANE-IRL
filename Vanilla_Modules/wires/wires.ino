#include "KTANE_Module.h"

#define VIN 5
#define BLUE 0
#define BLACK 1
#define WHITE 2
#define YELLOW 3
#define RED 4

//pins
const int ANALOUGEIN = 0;

//wire arrays
int wires2[6];
int wires[6]; 

int solution;

//resistor
float R1= 820;

//bomb serial number____________________________________________
//currently is hard coded add check later
bool serialOdd=true;

//multiplexor
const int analogControlPins[] = {5, 6, 7};
const int analogControlPinCount = 3;

KTANE_Module KTANE = KTANE_Module();

void setup() {
  Serial.begin(9600);

  //setup multiplexor pins
  for ( int i = 0; i < analogControlPinCount; i++) {
    pinMode(analogControlPins[i], OUTPUT);
  }

  KTANE.begin("Wires", 0, 1, 0); //Module name ver1, ver,2, ver3 (ver = version number)
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
    gameLoop();
  }

  if (KTANE.configMode() == true) { //could also replace this with (KTANE.gameMode() == false) as the module is either in config mode or game mode never both
    configLoop();
  }  
}

void gameLoop() {
  if (KTANE.armed() == false) {
    return; //if this module isn't armed then we don't need to do anything other than wait for a callback function to be called
    //callback functions are interupt based so we can just do nothing by returning.
  }
  
  if(detect(ANALOUGEIN,analogControlPinCount,analogControlPins,100,wires2)){
    bool differ=false;
    for (int i=0;i<6;i++){
      if (wires[i]!=wires2[i] && (wires[i]==5 || wires2[i]==5)){
        if (i==solution){
          KTANE.disarm();
        }else{
          KTANE.strike(1);
          Serial.print("/");
          for (int i=0;i<6;i++){
            wires[i]=wires2[i];
          }
        }
      }
    }
  }

  //The check for being armed has been made above so we know if we got to this point we're armed
  //do whatever we want to do to during the game to check if the player has put the module into a state
  //safe for disarming
}

void configLoop() {
  // if this is called the bomb is in config mode which means we want to know if it is in a valid config
  // we need to set this by using validConfig(bool isValid)
  
  detect(ANALOUGEIN,analogControlPinCount,analogControlPins,100,wires);
  solution=calculate(wires);
  
  if (solution !=-1) {
    solution=adjust(wires,solution);
    KTANE.setValidConfig(true);
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
  configLoop(); //don't need as should be looping but might as well update the config justin case
  if (KTANE.configValid() == true) {
    serialOdd=KTANE.serialOdd();
    //do anything here that you need done on arm
    Serial.println("ARMIFICATED");   
    return true;
  } else {
    Serial.println("Failed to ARM, config invalid. Try legging next time?");
    // write an error message here or something if you fail to arm for whatever reason
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

//_________________________________________________________________________
//custom functions

bool detect(int pin, int pinCount,int controlPins[],float offset,int wiresIn[]) {
  //for each potential wire
  for (int i=0;i<6;i++){
    
    //set the multiplexor
    for ( int j = 0; j < pinCount; j++) {
      digitalWrite(controlPins[j], i & (power(2, j)));
    }
    
    //read the multiplexor
    int raw = analogRead(pin);
    if (raw>0) {
      
      //calculate the resistance
      float temp = raw * VIN;
      float Vout = (temp) / 1024.0;
      temp = (VIN / Vout) - 1;
      float R2 = (R1 * temp)-offset;
      
      //detect wire colour
      if (R2 < 160) {
        wiresIn[i]= BLUE;
      }else if (R2 < 260) {
        wiresIn[i]= BLACK;
      }else if (R2 < 560) {
        wiresIn[i]= WHITE;
      }else if (R2 < 865) {
        wiresIn[i]= YELLOW;
      }else{
        wiresIn[i]= RED;
      }
    }else{
      wiresIn[i] = 5;//no wire
    }
  }
  return true;
}

int calculate(int wiresIn[]){
  switch (count(wiresIn, 5, 6)) {
    case 0:
    //6 valid wires
    if (count(wiresIn,YELLOW,6)==0&&serialOdd){return 3;}
    else if(count(wiresIn,YELLOW,6)==1&&count(wiresIn,WHITE,6)>1){return 4;}
    else if(count(wiresIn,RED,6)==0){return 6;}
    else {return 4;}
    
    break;
    case 1:
    //5 valid wires
    if (lastWire(wiresIn,BLACK,6)&&serialOdd){return 4;}
    else if (count(wiresIn,RED,6)==1&&count(wiresIn,YELLOW,6)>1){return 1;}
    else if(count(wiresIn,BLACK,6)==0){return 5;}
    else {return 2;}
    
    break;
    case 2:
    //4 valid wires
    if(count(wiresIn,RED,6)>1 && serialOdd){return lastOfType(wiresIn,RED,6);}
    else if (lastWire(wiresIn,YELLOW,6)&&count(wiresIn,RED,6)<1){return 1;}
    else if (count(wiresIn,BLUE,6)==1){return 1;}
    else if (count(wiresIn,YELLOW,6)>1){return 4;}
    else {return 2;}
    break;
    
    case 3:
    //3 valid wires
    if (count(wiresIn,RED,6)==0){return 2;}
    else if(lastWire(wiresIn, WHITE, 6)){return 3;}
    else if(count(wiresIn,BLUE,6)>1){return lastOfType(wiresIn,BLUE,6);}
    else {return 3;}
    break;
    
    default:
    return -1;
    break;
  }
}

//___________________________________________________________________
//utility functions

// power/exponent funtion that returns int
int power(int base, int exponent) {
  int solution = 1;
  for (int i = 0; i < exponent; i++) {
    solution *= base;
  }
  return solution;
}

//count number if input selector in array of length leng
int count(int in[], int selector, int leng) {
  int success = 0;
  for (int i = 0; i < leng; i++) {
    if (in[i] == selector) {
      success++;
    }
  }
  return success;
}

//find if the lalst wire of a sequence is your wire
bool lastWire(int in[], int selector, int leng) {
  for (int i = leng; i >= 0; i--) {
    if (in[i] != 5) {
      if (in[i] == selector) {
        return true;
      } else {
        return false;
      }
    }
  }
}

int lastOfType(int in[], int selector, int leng){
  int index=-1;
  for (int i=0;i<leng;i++){
    if (in[i]==selector){
      index=i;  
    }
  }
  return index;
}

int adjust(int wiresIn[], int wire){
  int count=0;
  for (int i=0;i<6;i++){
    if (wiresIn[i]!=5){
      count++;
    }
   if (count==wire){
    return i; 
   }
  }
}
