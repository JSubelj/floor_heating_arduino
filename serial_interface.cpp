#include "Arduino.h"


void getFromSerial(int * serialData){
    int new_temp;
    *serialData = 0;
    if(Serial.available()>0){
        String str = Serial.readString();
        Serial.println("GOT "+str);
        if(str[0] == 't'){
            new_temp = (str[1]-48)*10+(str[2]-48);
            *serialData = 1;
            *(serialData+1) = new_temp;
            if(str[3]!= ';'){
                *serialData = 0;
                Serial.println("Ukaz za spremebo temperature: 't{dvo mesta številka};'");
            }
            if(new_temp > 55){
                *serialData = 0;
                Serial.println("Temperatura ne sme biti višja kot 55 stopinj!");
            }
            if(*serialData){
                Serial.print("Temperatura je nastavljena na ");
                Serial.print(*(serialData+1));
                Serial.println(" °C");
            }
        }else{
            Serial.println("Ukaz za spremembo temperature: 't{dvo mesta številka};'");
        }
        
    }

}
