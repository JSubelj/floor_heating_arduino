// temp senzor: https://randomnerdtutorials.com/arduino-lm35-lm335-lm34-temperature-sensor/
#include "Arduino.h"
#include "circuit.h"
#include "shared.h"
#include "configuration.h"
#include "sensors_and_relays.h"
#include "mixed_valve_regulation.h"
#include "serial_interface.h"



// Taski:
//  mer temperaturo
//  spreme regulacijo
//  prdob podatke iz 


void setup(){
    Serial.begin(115200);
    while (!Serial);  
    pinMode(TEMP_FURNICE, INPUT);
    pinMode(TEMP_FLOOR_INLET, INPUT);
    pinMode(TEMP_FLOOR_OUTLET, INPUT);
    pinMode(RELAY_DECREASE_TEMP, OUTPUT);
    pinMode(RELAY_INCREASE_TEMP, OUTPUT);
    pinMode(RELAY_PUMP, OUTPUT);
    pinMode(MIXER_VALVE_LED, OUTPUT);

    digitalWrite(RELAY_DECREASE_TEMP, 1);
    digitalWrite(RELAY_INCREASE_TEMP, 1);
    digitalWrite(RELAY_PUMP, 1);
    digitalWrite(MIXER_VALVE_LED, 0);
    
    
}


int temp_wanted = INITIAL_TEMPERATURE;

int TOO_HOT = false;
unsigned long start_time = millis();

void loop(){
  // 3600mV pri 100°C 373K
  #define sensorPin A0

  float sensorValue = analogRead(A0);
  float voltageOut1 = (sensorValue*5000)/10240 -273;
   sensorValue = analogRead(A1);
  float voltageOut2 = (sensorValue*5000)/10240 -273;
   sensorValue = analogRead(A2);
  float voltageOut3 = (sensorValue*5000)/10240 -273;

  Serial.print("a0: ");
  Serial.print(voltageOut1);
  Serial.print(" a1: ");
  Serial.print(voltageOut2);
  Serial.print(" a2: ");
  Serial.println(voltageOut3);
  delay(1000);
  /*
  
  delay(200);
  sensorValue = analogRead(sensorPin);
  float voltageOut2 = (sensorValue*5000)/1024;
  delay(200);
  sensorValue = analogRead(sensorPin);
  float voltageOut3 = (sensorValue*5000)/1024;
  delay(200);
  sensorValue = analogRead(sensorPin);
  float voltageOut4 = (sensorValue*5000)/1024;
  delay(200);
  sensorValue = analogRead(sensorPin);
  float voltageOut5 = (sensorValue*5000)/1024;
  delay(200);
  Serial.println((voltageOut1+voltageOut2+voltageOut3+voltageOut4+voltageOut5)/5);*/
  /*  float temp_floor_inlet    = getFloorInletTemp();
    float temp_floor_outlet   = getFloorOutletTemp();
    float temp_furnice        = getFurniceTemp();

    control(temp_wanted, temp_floor_inlet, temp_floor_outlet, temp_furnice);

    int serialData[2]= {};
    getFromSerial(serialData);
    if(serialData[0]){
      temp_wanted = serialData[1];
    }

    if(getFloorInletTemp() >= MAX_INLET_TEMP){
        TOO_HOT = true;
    }

    if(millis()-start_time >1000){
        Serial.print("Wanted temperature: ");
        Serial.print(temp_wanted);
        Serial.print("; Temperature floor inlet: ");
        Serial.print(temp_floor_inlet);
        Serial.print("; Temperature floor outlet: ");
        Serial.print(temp_floor_outlet);
        Serial.print("; Temperature furnice: ");
        Serial.println(temp_furnice);
        Serial.print("Relay mixer decrease: ");
        Serial.print(!digitalRead(RELAY_DECREASE_TEMP));
        Serial.print("; Relay mixer increase: ");
        Serial.print(!digitalRead(RELAY_INCREASE_TEMP));
        Serial.print("; Relay pump: ");
        Serial.println(!digitalRead(RELAY_PUMP));
        Serial.print("Current position: ");
        Serial.print(current_position_mix_valve);
        Serial.print("; Wanted position: ");
        Serial.println(wanted_position);
        Serial.println();
        Serial.flush();
        start_time = millis();
    }
*/
    /*int tempOnLine = readTempInCel();
    
    // prižgi pumpo ko je temperatura manjša od nastavljene
    if(tempOnLine < temperatureSet){
        digitalprint(pumpPin,1);
    }


    if(abs(temperatureSet-tempOnLine) <= 3){
        // smo v območju histereze, dela naj samo pumpa
        digitalprint(lowerTempPin, 0);
        digitalprint(increaseTempPin, 0);
    }else if(tempOnLine < temperatureSet) {
        // premau je tkoda je treba odpret ventil
        digitalprint(lowerTempPin,0);
        digitalprint(increaseTempPin,1);
    }else if(tempOnLine > temperatureSet){
        if(tempOnLine > temperatureSet+4){
            digitalprint(pumpPin,0);
        }
        digitalprint(increaseTempPin,0);
        digitalprint(lowerTempPin,1);
    }
    
    Serial.print("Temperature (°C): ");
    Serial.println(tempOnLine);
    Serial.print("pin to lower temp set to: ");
    Serial.println(digitalRead(lowerTempPin));
    Serial.print("pin to increase temp set to: ");
    Serial.println(digitalRead(increaseTempPin));
    Serial.print("pin for pump set to: ");
    Serial.println(digitalRead(pumpPin));

    if(digitalRead(lowerTempPin) && digitalRead(increaseTempPin)){
        digitalprint(lowerTempPin,1);
        digitalprint(increaseTempPin,0);
    }
    char savingChars[3];
    while (Serial.available() > 0) {
    // read the incoming byte:
        int incomingByte = Serial.read();
        if(incomingByte == 'T'){
            tempRecieving = 1;
        }else if(tempRecieving == 1){
            savingChars[0] = incomingByte;
            tempRecieving++;
        }else if(tempRecieving == 2){
            savingChars[1] = incomingByte;
            savingChars[2] = 0;
            tempRecieving++;
        }else{
            tempRecieving = -1;
        }

    }
    if(tempRecieving == 3){
        String str(savingChars);
        str = savingChars;
        temperatureSet = str.toInt();
        tempRecieving = 0;
    }else if(tempRecieving == 0){
        savingChars[0] = 0;
        savingChars[1] = 0;
        savingChars[2] = 0;
    }else{
        Serial.println("Za nastavitev temperature vpišite: T{temp} npr.: T20");
        tempRecieving=0;
    }
    delay(10);*/




}
