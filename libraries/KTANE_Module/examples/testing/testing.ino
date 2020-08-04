#include <KTANE_Module.h>
KTANE_Module KTANE = KTANE_Module();


void setup() {
  
  Serial.begin(9600);
  KTANE.begin("testing", 0, 1, 0);
  Serial.println(KTANE.batteries());
  Serial.println(KTANE.port());
  Serial.println(KTANE.serialOdd());
  Serial.println(KTANE.serialEven());
  Serial.println(KTANE.indicator(0));

}

void loop(){
  Serial.println(KTANE.timeTillDetonation());
  delay(1000);
}
 
