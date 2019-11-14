#include "Arduino.h"
#include "circuit.h"
#include "shared.h"
#include "configuration.h"
#include "sensors_and_relays.h"

int current_position_mix_valve = 0;
float current_position_float = 0;
float temp_correction = TEMPERATURE_CORRECTION;

float _readTempInCel(int sensorPin){
  float sensorValue = analogRead(sensorPin);
  return (sensorValue*5000)/10230 -273 + temp_correction;
}

float getFloorInletTemp(){
    return _readTempInCel(TEMP_FLOOR_INLET);
}

float getFloorOutletTemp(){
    return _readTempInCel(TEMP_FLOOR_OUTLET);
}

float getFurniceTemp(){
    return _readTempInCel(TEMP_FURNICE);
}

struct timer_t {
   bool operational;
   unsigned long start_time;
};  

struct timer_t timer_close_mix_valve { .operational = false, .start_time =0 };
struct timer_t timer_open_mix_valve_one_step { .operational = false, .start_time =0 };
struct timer_t timer_close_mix_valve_one_step { .operational = false, .start_time =0 };
struct timer_t timer_open_mix_valve_for_time { .operational = false, .start_time =0 };
struct timer_t timer_close_mix_valve_for_time { .operational = false, .start_time =0 };


int closeMixValve(){
    decreaseTemp();
    if(millis() - timer_close_mix_valve.start_time > TIME_MIN_MAX_MIXER_MS * 1.5){
        stopMixValve();
        current_position_mix_valve = 0;
        timer_close_mix_valve.operational = false;
        return 1;
    }
    return 0;
    
}

int openMixValveOneStep(){
    increaseTemp();
    if(millis() - timer_open_mix_valve_one_step.start_time > TIME_FOR_STEP_MS){
        timer_open_mix_valve_one_step.operational = false;
        stopMixValve();
        current_position_mix_valve++;
        current_position_mix_valve = current_position_mix_valve > STEPS ? STEPS : current_position_mix_valve;
        return 1;
    }
    return 0;    
    
}

int openMixValveForTime(int ms_time){
    increaseTemp();
    if(millis() - timer_open_mix_valve_for_time.start_time >= ms_time){
        timer_open_mix_valve_for_time.operational = false;
        stopMixValve();
        return 1;
    }
    return 0;    

}

int closeMixValveForTime(int ms_time){
    decreaseTemp();
    if(millis() - timer_close_mix_valve_for_time.start_time >= ms_time){
        timer_close_mix_valve_for_time.operational = false;
        stopMixValve();
        return 1;
    }
    return 0;    

}


int setToPositionFloat(float position){
    float positions_to_move = current_position_float-position;

    if(position == 0){
        if(!timer_close_mix_valve.operational){
            timer_close_mix_valve.operational = true;
            timer_close_mix_valve.start_time = millis();
        }
        int ret = closeMixValve();
        return ret && position == current_position_mix_valve;
    }

    if(positions_to_move < 0){
        positions_to_move*=-1;

        float time_to_spend = positions_to_move*TIME_MIN_MAX_MIXER_MS;
        if(!timer_close_mix_valve_for_time.operational){
            timer_close_mix_valve_for_time.operational = true;
            timer_close_mix_valve_for_time.start_time = millis();
        }
        int ret = closeMixValveForTime(time_to_spend);
        current_position_float = position;
        return ret;
    }

    if(positions_to_move > 0){
        float time_to_spend = positions_to_move*TIME_MIN_MAX_MIXER_MS;
        if(!timer_open_mix_valve_for_time.operational){
            timer_open_mix_valve_for_time.operational = true;
            timer_open_mix_valve_for_time.start_time = millis();
        }
        int ret = openMixValveForTime(time_to_spend);
        current_position_float = position;
        return ret;
    }
    return 1;

    

}

int setToPosition(int position){
    int positions_to_move = current_position_mix_valve-position;

    if(position == 0){
        if(!timer_close_mix_valve.operational){
            timer_close_mix_valve.operational = true;
            timer_close_mix_valve.start_time = millis();
        }
        int ret = closeMixValve();
        return ret && position == current_position_mix_valve;
    }


    if(positions_to_move < 0){
        if(!timer_open_mix_valve_one_step.operational){
            timer_open_mix_valve_one_step.operational = true;
            timer_open_mix_valve_one_step.start_time = millis();
        }
        int ret = openMixValveOneStep();
        return ret && position == current_position_mix_valve;
        
    }else if(positions_to_move > 0){
        if(!timer_close_mix_valve_one_step.operational){
            timer_close_mix_valve_one_step.operational = true;
            timer_close_mix_valve_one_step.start_time = millis();
        }
        
        int ret = closeMixValveOneStep();
        return ret && position == current_position_mix_valve;
        
    }
    return 1;
}

int closeMixValveOneStep(){
    

    decreaseTemp();
    if(millis() - timer_close_mix_valve_one_step.start_time > TIME_FOR_STEP_MS){
        stopMixValve();

        current_position_mix_valve--;
        current_position_mix_valve = current_position_mix_valve < 0 ? 0 : current_position_mix_valve;
        timer_close_mix_valve_one_step.operational = false;
        return 1;
    }
    return 0;
}

/*void turnOnIndicatorLight(){
    digitalWrite(MIXER_VALVE_LED,1);
}


void turnOffIndicatorLight(){
    digitalWrite(MIXER_VALVE_LED,0);
}*/

void increaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,1);
    digitalWrite(RELAY_INCREASE_TEMP,0);
}

void decreaseTemp(){
    digitalWrite(RELAY_DECREASE_TEMP,0);
    digitalWrite(RELAY_INCREASE_TEMP,1);
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
