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

struct timer_t {
   bool operational;
   unsigned long start_time;
};  

struct timer_t timer_close_mix_valve { .operational = false, .start_time =0 };
struct timer_t timer_open_mix_valve_one_step { .operational = false, .start_time =0 };
struct timer_t timer_close_mix_valve_one_step { .operational = false, .start_time =0 };


int closeMixValve(){
    turnOnMixValveLight();
    decreaseTemp();
    if(millis() - timer_close_mix_valve.start_time > TIME_MIN_MAX_MIXER_MS * 1.5){
        stopMixValve();
        turnOffMixValveLight();
        position_mix_valve = 0;
        timer_close_mix_valve.operational = false;
        return 1;
    }
    return 0;
    
}

int openMixValveOneStep(){
    turnOnMixValveLight();
    increaseTemp();
    if(millis() - timer_open_mix_valve_one_step.start_time > TIME_FOR_STEP_MS){
        timer_open_mix_valve_one_step.operational = false;
        stopMixValve();
        turnOffMixValveLight();
        position_mix_valve++;
        position_mix_valve = position_mix_valve > STEPS ? STEPS : position_mix_valve;
        return 1;
    }
    return 0;    
    
}

void setToPosition(int position){
    int positions_to_move = position_mix_valve-position;

    if(position == 0){
        if(!timer_close_mix_valve.operational){
            timer_close_mix_valve.operational = true;
            timer_close_mix_valve.start_time = millis();
        }
        int ret = closeMixValve();
        return ret && position == position_mix_valve;
    }

    if(positions_to_move < 0){
        if(!timer_open_mix_valve_one_step.operational){
            timer_open_mix_valve_one_step.operational = true;
            timer_open_mix_valve_one_step.start_time = millis();
        }
        int ret = openMixValveOneStep();
        return ret && position == position_mix_valve;
        
    }else if(positions_to_move > 0){
        if(!timer_close_mix_valve_one_step.operational){
            timer_close_mix_valve_one_step.operational = true;
            timer_close_mix_valve_one_step.start_time = millis();
        }
        
        int ret = closeMixValveOneStep();
        return ret && position == position_mix_valve;
        
    }
    return 1;
}

int closeMixValveOneStep(){
    
    turnOnMixValveLight();

    decreaseTemp();
    if(millis() - timer_close_mix_valve_one_step.start_time > TIME_FOR_STEP_MS){
        stopMixValve();
        turnOffMixValveLight();

        position_mix_valve--;
        position_mix_valve = position_mix_valve < 0 ? 0 : position_mix_valve;
        timer_close_mix_valve_one_step.operational = false;
        return 1;
    }
    return 0;
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