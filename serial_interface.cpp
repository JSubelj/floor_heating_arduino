#include "Arduino.h"
#include <string.h>
#include <EEPROM.h>

void EEPROMSaveConfig(int temp, float correct){
    EEPROM.put(0,temp);
    EEPROM.put(sizeof(int),correct);
}

void EEPROMLoadConfig(int *temp, float *correct){
    EEPROM.get(0, temp);
    EEPROM.get(sizeof(int),correct);
}



void getFromSerial(float * serialData){
    int new_temp;
    float new_c;
    *serialData = 0;
    char command[30]; 
    if(Serial.available()>0){
        Serial.readBytes(command, 29);
        *(strchr( command, ';' )+1) = 0;
        switch((int)command[0]){
            case 't':{
                *serialData=1;
                if('0' > command[1] || '9' < command [1] && '0' > command[2] && '9' < command [2] && ';' != command[3]){
                }else{*serialData = 0;}
                new_temp = (command[1]-'0')*10+(command[2]-'0');
                Serial.print("Got command: ");
                Serial.print(command);
                Serial.print(" | Temperature set: ");
                Serial.println(new_temp);
                if(new_temp > 55 || new_temp < 10 || new_temp % 5 !=0 ){
                    *serialData = 0;           
                }
                if(*serialData == 0){
                    Serial.println("Temperatura mora biti med 10 in 55 stopinj in deljiva s 5");
                    return;
                }
                *(serialData+1) = new_temp;
                return;

            }
            case 'c':{
                *serialData = 2;
                char * podpicje_inx = strchr( command, ';' );
                
                if(podpicje_inx == NULL){
                    *serialData=0;
                }
                * podpicje_inx = 0;
                float val = atof(command+1);

                if (val > 20 || val < -20){
                    *serialData=0;
                }
                int tmp = val*10;
                if(tmp%5 != 0){
                    *serialData=0;
                }
                val = tmp / 10.0;
                Serial.print("Got command: ");
                Serial.print(command);
                Serial.print(" | Correction set: ");
                Serial.println(val);
                
                if(*serialData == 0){
                    Serial.println("Korekcija mora biti med -20 in 20 stopinj in deljiva s 0.5");
                    return;
                }
                *(serialData+1) = val;
				return;

            }
            default: {
                *serialData = 0;
                Serial.println("Ukaz za spremembo temperature: 't{dvo mesta številka deljiva s 5};'");
                Serial.println("Ukaz za popravek temperature: 'c{oblika 20.0};");
                break;
            }
        }
    }
}
/*
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
*/