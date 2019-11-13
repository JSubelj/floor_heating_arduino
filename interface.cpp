#include "circuit.h"
#include "Arduino.h"

#define  ON 0
#define OFF 1

void _turn_off_all_leds(){
    for(int i=0; all_LEDs[i] != -1; i++){
        digitalWrite( all_LEDs[i], OFF );
    }
    
}

void set_leds(int temp){
    if(temp>=10 && temp <=55){
        int what_leds_to_turn_on = (temp - 10) / 5;
        _turn_off_all_leds();
        for(int i=0; i<what_leds_to_turn_on ; i++){
            digitalWrite(all_LEDs[i], ON);
        }
    }
}

void read_input(int wanted_temp){
    static int old_state_up = 1;
    static int old_state_down = 1;
    int new_state = digitalRead(BUTTON_TEMP_UP)
    if(old_state_up = 1 && new_state == 0){
        wanted_temp += 5;
    }
    old_state_up = new_state;

    new_state = digitalRead(BUTTON_TEMP_DOWN);
    if(old_state_down = 1 && new_state == 0){
        wanted_temp -= 5;
    }
    old_state_down = new_state;

    wanted_temp = wanted_temp > 55? 55 : wanted_temp;
    wanted_temp = wanted_temp < 10? 10 : wanted_temp;

    return wanted_temp;
}