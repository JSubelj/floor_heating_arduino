#include "Arduino.h"
#include "../circuit.h"

int pins[] = {A,B,C,D,E,F,G,DP};
int digits[] = {INDICATOR_7_DRAIN, LEFT_7_DRAIN, RIGHT_7_DRAIN};

void setup(){
    for(int i=0; i< 8;i++){
        pinMode(pins[i], OUTPUT);
    }
    for(int i=0; i< 3;i++){
        pinMode(digits[i], OUTPUT);
    }
}

unsigned int start_time = millis();
int on_pin=0;
int on_digit=0;
void loop(){

    for(int i=0; i<3;i++){
        digitalWrite(on_digit,1);
        on_digit = i;
        digitalWrite(on_digit,0);
        for(int j=0; j<8;j++){
            digitalWrite(on_pin, 0);
            on_pin = j;
            digitalWrite(on_pin, 1);
            delay(1000);
            
        }
    }


}