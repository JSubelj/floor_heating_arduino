#include "Arduino.h"
#include "../circuit.h"
#include "SevSeg.h"

SevSeg sevseg;


byte pins[] = {A,B,C,D,E,F,G,DP};
byte digits[] = {INDICATOR_7_DRAIN, LEFT_7_DRAIN, RIGHT_7_DRAIN};

void setup(){
    byte numDigits = 3;
 
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE ; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]
    sevseg.begin(hardwareConfig, numDigits, digits, pins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
}

unsigned int start_time = millis();
int on_pin=0;
int on_digit=0;
int i = 0;
void loop(){

    while(i<1000){
        if(millis()-start_time > 250){
            sevseg.setNumber(i);
            start_time = millis();
            i++;
        }
    }


}