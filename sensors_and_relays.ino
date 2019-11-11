#include "circuit.h"

int _readTempInCel(int sensorPin){
    return analogRead(sensorPin)/10 -273;
}

int getFloorIntakeTemp(){
    return _readTempInCel(TEMP_FLOOR_INTAKE);
}

int getFloorOutletTemp(){
    return _readTempInCel(TEMP_FLOOR_OUTLET);
}

int getFurniceInletTemp(){
    return _readTempInCel(TEMP_FURNICE_INTAKE);
}


void increaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,0);
}

void decreaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,0);
}

void startPump(){
    digitalWrite(RELAY_PUMP,0);
}

void stopPump(){
    digitalWrite(RELAY_PUMP,1);
}