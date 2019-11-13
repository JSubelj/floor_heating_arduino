#include "Arduino.h"
#include <string.h>
#include <EEPROM.h>

void EEPROMSaveConfig(int temp, int correct){
    EEPROM.put(0,temp);
    EEPROM.put(sizeof(int),correct);
}

void EEPROMLoadConfig(int *temp, int *correct){
    EEPROM.get(0, temp);
    EEPROM.get(sizeof(int),correct);
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
                Serial.println("Ukaz za spremebo temperature: 't{dvo mesta številka deljiva s 5};'");
            }
            if(new_temp > 55){
                *serialData = 0;
                Serial.println("Temperatura ne sme biti višja kot 55 stopinj!");
            }
            if(new_temp % 5 != 0){
                Serial.println("Temperatura mora biti deljiva s 5!");
            }

            if(*serialData){
                Serial.print("Temperatura je nastavljena na ");
                Serial.print(*(serialData+1));
                Serial.println(" °C");
            }
        }else if(str[0] == 'c'){
            char ss[7];
            *serialData = 0;
            const char * str_tmp = str.c_str();
            strncpy(ss, (str_tmp+1),6);
            bool found_end = false;
            for(int i=0;i<7;i++){
                if(ss[i] == ';'){
                    ss[i] = 0;
                    found_end = true;
                    break;
                }
            }
            
            if(!found_end){
                Serial.println("Ukaz za popravek temperature: 'c{4 mestna stevilka};");
            }
            *serialData = 2;
            *(serialData+1) = atoi(ss);
            Serial.println(ss);

        }
        
        else{
            Serial.println("Ukaz za spremembo temperature: 't{dvo mesta številka deljiva s 5};'");
            Serial.println("Ukaz za popravek temperature: 'c{4 mestna stevilka};");

        }
        
    }

}
