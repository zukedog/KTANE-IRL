#include <Wire.h>
#include <KTANE_Controller.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(2000000);  // start serial for output

  // reserve 200 bytes for the inputString:
  inputString.reserve(10);
}

byte x = 0;


byte getStatus() {
  Wire.beginTransmission(8);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte stat = Wire.read();
  return stat;
}

void getConfig() {
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte size = Wire.read();

  Wire.requestFrom(8, size);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println();
}

void getError() {
  Wire.beginTransmission(8);
  Wire.write(2);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte size = Wire.read();
  Wire.requestFrom(8, size);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println();
}

void getDebug() {
  Wire.beginTransmission(8);
  Wire.write(3);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte size = Wire.read();
  Wire.requestFrom(8, size);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  //Serial.println();
}

void getStrikes() {
  Wire.beginTransmission(8);
  Wire.write(4);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte strikes = Wire.read();
  Serial.println(strikes);
}

void getName() {
  Wire.beginTransmission(8);
  Wire.write(5);
  Wire.endTransmission();
  Wire.requestFrom(8, 1);
  byte size = Wire.read();
  Wire.requestFrom(8, size);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println();
}

void getVersion() {
  Wire.beginTransmission(8);
  Wire.write(6);
  Wire.endTransmission();
  Wire.requestFrom(8, 3);
  while (Wire.available()) { // slave may send less than requested
    int i = Wire.read(); // receive a byte as character
    Serial.print(i);         // print the character
    Serial.print(".");
  }
  Serial.println();
}


byte status;
byte oldStatus;

void loop() {
  status = getStatus();
  if (status != B11111111) {
    if (status != oldStatus) {
      Serial.println("NEW STATUS RECEIVED");
      Serial.print("GM: ");
      Serial.print((status & B10000000) > 0);
      Serial.print(", VC/A: ");
      Serial.print((status & B01000000) > 0);
      Serial.print(", NFC: ");
      Serial.print((status & B00100000) > 0);
      Serial.print(", ERR: ");
      Serial.print((status & B00010000) > 0);
      Serial.print(", DBG: ");
      Serial.print((status & B00001000) > 0);
      Serial.print(", STR: ");
      Serial.print((status & B00000100) > 0);
      Serial.print(", WGS: ");
      Serial.print((status & B00000010) > 0);
      Serial.print(", ???: ");
      Serial.println((status & B00000001) > 0);
      oldStatus = status;
      Serial.println("--------------------------------------------------------------");
    }
    if (status & B00001000) {
      Serial.print("DEBUG: ");
      getDebug();
      Serial.println("--------------------------------------------------------------");
    }
  }




  if (stringComplete) {
    if (inputString == "arm\n") {
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(20);
      Wire.endTransmission();    // stop transmitting
    }

    if (inputString == "reset\n") {
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(21);
      Wire.endTransmission();    // stop transmitting
    }

    if (inputString == "explode\n") {
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(22);
      Wire.endTransmission();    // stop transmitting
    }

    if (inputString == "win\n") {
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(23);
      Wire.endTransmission();    // stop transmitting
    }

    if (inputString == "getStatus\n") {
      Serial.println(getStatus(), BIN);
    }

    if (inputString == "getConfig\n") {
      getConfig();
    }

    if (inputString == "getError\n") {
      getError();
    }

    if (inputString == "getDebug\n") {
      getDebug();
    }

    if (inputString == "getStrikes\n") {
      getStrikes();
    }

    if (inputString == "getName\n") {
      getName();
    }

    if (inputString == "getVersion\n") {
      getVersion();
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
