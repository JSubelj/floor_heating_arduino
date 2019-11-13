#include "circuit.h"
#include "Arduino.h"

#define  ON 1
#define OFF 0

void _turn_off_all_leds(){
    for(int i=0; all_LEDs[i] != -1; i++){
        digitalWrite( all_LEDs[i], OFF );
    }
    
}

void _turn_on_n_leds(int n){
    for(int i=0; i<n ; i++){
        digitalWrite( all_LEDs[i], ON );
    }
    
}


int step = 0;
int time_start = millis();
void set_leds(int temp,int mode){
  if(step ==0) {time_start = millis();_turn_off_all_leds();}
    switch(mode){
      case 1: {
        
        
        switch(step){
          case 0: {digitalWrite(INDICATOR_LED, 1); step++; break;}
          case 1: {if(millis()-time_start >= 500){
            digitalWrite(INDICATOR_LED, 0);
            time_start=millis();
            step++;
          }break;}
          case 2: {if(millis()-time_start >= 500){
            time_start=millis();
            _turn_on_n_leds(temp/10);
            step++;}
            break; }           
          
          case 3: {if(millis()-time_start >= 2000){
            time_start=millis();
            _turn_off_all_leds();
            step++;}
            break; }
          
          case 4:{ if(millis()-time_start >= 2000){
            time_start=millis();
            _turn_on_n_leds(temp%10);
            
            step++;
            
          }break; }
          case 5:{
          if(millis()-time_start >= 2000){
            time_start=0;
            time_start=millis();

            _turn_off_all_leds();
            step++;
            }break;
          case 6:{
            if(millis()-time_start >= 500){
              step=0; 
            }
          }
            
          }break;}
        }
        default:{
          break;
        }
      }
    }
    /*if(temp>=10 && temp <=55){
        int what_leds_to_turn_on = (temp - 10) / 5;
        _turn_off_all_leds();
        for(int i=0; i<what_leds_to_turn_on ; i++){
            digitalWrite(all_LEDs[i], ON);
        }
    }*/


int read_input_down(int wanted_temp){
    static int old_state_down = 1;
    int new_state1 = digitalRead(BUTTON_TEMP_DOWN);

    if(old_state_down ==1 && new_state1 == 0){
        wanted_temp -= 5;
          Serial.println("down");

    }
    old_state_down=new_state1;
    if(wanted_temp < 10){
      wanted_temp=10;
    }
    return wanted_temp;
    }

int read_input_up(int wanted_temp){
    static int old_state_up = 1;

    int new_state = digitalRead(BUTTON_TEMP_UP);
    if(old_state_up == 1 && new_state == 0){
        wanted_temp += 5;
                  Serial.println("up");

    }
    old_state_up = new_state;

    

     
    if(wanted_temp > 55){
      wanted_temp= 55;
    }

    return wanted_temp;
}
