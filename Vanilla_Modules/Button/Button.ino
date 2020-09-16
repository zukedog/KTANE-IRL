#include <KTANE_Module.h>
#include <Adafruit_NeoPixel.h>

int LED_PIN=9;

KTANE_Module KTANE = KTANE_Module();
Adafruit_NeoPixel strip(3, LED_PIN, NEO_RGB + NEO_KHZ800);

//defuse variables
bool HOLD=false;
String Release="0";

//pins for specific things
const int led=4;
const int push=7;

//threshold for push and relese
const int threshold=500;

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

  strip.begin();

  randomSeed(analogRead(1));
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
    Release=temp[random(0,3)];
  }
  return true;
}

bool ifPress(String timing, bool mode){
  if(!digitalRead(push)){
    start=millis();

    for (int i=0;i<3;i++){
      strip.setPixelColor(i, strip.Color(colours[Release.toInt()][0], colours[Release.toInt()][1], colours[Release.toInt()][2]));
    }
    
    while(!digitalRead(push)){
      if ((millis()-start)>500 && (millis()-start)<510){
        strip.show();
      }
      delay(1);
    }

    for (int i=0;i<3;i++){
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();
    
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
