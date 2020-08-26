#include <KTANE_Module.h>

KTANE_Module KTANE = KTANE_Module();

//defuse variables
bool HOLD=false;
String Release="0";

//pins for specific things
const int led=4;
const int push=7;

//threshold for push and relese
const int threshold=100;

unsigned long start;

//colours due to their relese number
int colours[6][3] ={{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,255},{0,255,255}};

void setup() {
  //sets up the pins for button ID
  Serial.begin(9600);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);

  //setup button push pin
  pinMode(push,INPUT_PULLUP);

  //setup led pins
  pinMode(led,OUTPUT);

  KTANE.begin("Button",0,0,1);
  KTANE.onArm(detectButton);
}

void loop() {
  KTANE.setValidConfig(true);
  if (KTANE.gameMode()){ 
    ifPress(Release,HOLD);
  }
}

bool detectButton(){
//pin inputs from the button circle that mean press and release
// 4 = detonate
// 5 = red/hold
// 6 = white - blue/Abort

//if any of the above cases
  if((KTANE.batteries()>1 && !digitalRead(4))||!digitalRead(5)||(KTANE.indicator(10) && (digitalRead(6) && !KTANE.indicator(2)))){
    //set to press and relese
    KTANE.debugln("click");
    HOLD=false;
  }else{
    HOLD=true;
    KTANE.debugln("hold");
    //pick colour for the lit indicator
    String temp[]={"1","4","5"};
    Release=temp[random(0,2)];
  }
  return true;
}

bool ifPress(String timing, bool mode){
  if(!digitalRead(push)){
    start=millis();
    
    while(!digitalRead(push)){
      if ((millis()-start>5000) || (millis()-start<6000)){
        //show indicator strip
      }
      delay(1);
    }
    
    //calculate numbers in time (miliseconds to dentination)to find the exact symbols that are on the clock
    unsigned long Time= KTANE.timeTillDetonation()/1000;
    
    String symbols=(String)(Time/60);//get minutes
    
    symbols+=(String)(Time%60);//get seconds

    //if there is a number in the time (and it is press and hold) or it is within the threshold (press and relese)
    if (((symbols.indexOf(timing) != -1) && mode) || (((millis()-start)<threshold) && !mode)){
      KTANE.disarm();
    }else{
      KTANE.strike(); 
    }       
  }
}
