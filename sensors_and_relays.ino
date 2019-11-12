#include "circuit.h"

int position_mix_valve = 0;

int _readTempInCel(int sensorPin){
    return analogRead(sensorPin)/10 -273;
}

int getFloorInletTemp(){
    return _readTempInCel(TEMP_FLOOR_INTAKE);
}

int getFloorOutletTemp(){
    return _readTempInCel(TEMP_FLOOR_OUTLET);
}

int getFurniceTemp(){
    return _readTempInCel(TEMP_FURNICE);
}

void closeMixValve(){
    int start_time = millis();
    turnOnMixValveLight();
    decreaseTemp();
    while(millis() - start_time < TIME_MIN_MAX_MIXER_MS * 1.5);
    stopMixValve();
    turnOffMixValveLight();

    position_mix_valve = 0;
}

void openMixValveOneStep(){
    int start_time = millis();
    turnOnMixValveLight();
    increaseTemp();
    while(millis() - start_time < TIME_FOR_STEP_MS);
    stopMixValve();
    turnOffMixValveLight();
    position_mix_valve++;
    
}

void setToPosition(int position){
    int positions_to_move = position_mix_valve-position;
    if(positions_to_move < 0){
        positions_to_move*=1;
        for(int x=0;x<positions_to_move;x++ ){
            openMixValveOneStep();
        }
    }else if(positions_to_move > 0){
        for(int x=0;x<positions_to_move;x++ ){
            closeMixValveOneStep();
        }
    }
}

void closeMixValveOneStep(){
    unsigned long start_time = millis();
    turnOnMixValveLight();

    decreaseTemp();
    while(millis() - start_time < TIME_FOR_STEP_MS);
    stopMixValve();
    turnOffMixValveLight();

    position_mix_valve--;
}

void turnOnMixValveLight(){
    digitalWrite(MIXER_VALVE_LED,1);
}


void turnOffMixValveLight(){
    digitalWrite(MIXER_VALVE_LED,0);
}

void increaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,0);
}

void decreaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,0);
}

void stopMixValve(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,1);
}

void startPump(){
    digitalWrite(RELAY_PUMP,0);
}

void stopPump(){
    digitalWrite(RELAY_PUMP,1);
}