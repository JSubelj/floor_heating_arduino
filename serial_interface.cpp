#include "Arduino.h"
#include <string.h>

void EEPROMSaveConfig(struct EEPROMData *data){
    data->save_indicator = 1488;
    EEPROM.put(0,data);
}

void EEPROMLoadConfig(struct EEPROMData *data){
    EEPROM.get(0, data);
}

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
        }else if(str[0] == 'c'){
            char substring[7];
            *serialData = 0;
            strncpy(substring, (&str+1),6);
            bool found_end = false;
            for(int i=0;i<7;i++){
                if(substring[i] == ';'){
                    substring[i] = 0;
                    found_end = true;
                    break;
                }
            }
            
            if(!found_end){
                Serial.println("Ukaz za popravek temperature: 'c{4 mestna stevilka};");
            }
            *serialData = 2;
            *(serialData+1) = atoi(substring);

        }
        
        else{
            Serial.println("Ukaz za spremembo temperature: 't{dvo mesta številka};'");
            Serial.println("Ukaz za popravek temperature: 'c{4 mestna stevilka};");

        }
        
    }

}
