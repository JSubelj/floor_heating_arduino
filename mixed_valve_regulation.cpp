#include "Arduino.h"
#include "configuration.h"
#include "circuit.h"
#include "shared.h"
#include "sensors_and_relays.h"

int initialisation(){
    unsigned long start_time = millis();
    if(closeMixValve()){
      startPump();
      return 1;
    }
    return 0;
}

int calculatePositionOfMixValve(int temp_wanted, int temp_floor_inlet, int temp_floor_outlet, int temp_furnice){
    temp_wanted += MIN_DELTA_INLET_OUTLET_DEG;
    float b = (temp_wanted - temp_floor_outlet);
    float a = (temp_furnice - temp_wanted);
    float procentage = b/(a+b);
    procentage*=100;
    int lower = (int) procentage / STEP_SIZE;
    int upper = lower+1;
    int ret;
    if((int)procentage % STEP_SIZE > STEP_SIZE/2.0) {
        ret= upper;
    }else {
        ret = lower;
    }

    return ret > STEPS ? STEPS : ret < 0 ? 0 : ret;
}

int state=0;
int old_position = 0;
bool newValue = true;
int wanted_position = 0;
unsigned long start_waiting;

void control(int temp_wanted, int temp_floor_inlet, int temp_floor_outlet, int temp_furnice){
    switch (state)
    {
    case 0:{
        // initialization
        if(initialisation()){
          state++;
        }
        break;
    }
    case 1:{
        // running
        if(temp_floor_inlet < MAX_INLET_TEMP-5){
            startPump();
            
        }else if(temp_floor_inlet >= MAX_INLET_TEMP){
            stopPump();
        }
        if(newValue){
            old_position = wanted_position;
            wanted_position = calculatePositionOfMixValve(temp_wanted, temp_floor_inlet, temp_floor_outlet, temp_furnice);
            if(wanted_position <= old_position && temp_floor_inlet < temp_wanted){
                wanted_position = old_position+1;
            }
            newValue = false;
        }
        if(setToPosition(wanted_position)){
            newValue = true;
            state++;
        }        
        
        break;
    }
    case 2:{
        // waiting state
        start_waiting = millis();
        state++;
        break;
    }
    case 3:{
        if(millis() - start_waiting > WAIT_BETWEEN_CHANGES_OF_REGULATION_MS){
            state = 1;
        }
        break;  	    
    }
    default:
        break;
    }
}
