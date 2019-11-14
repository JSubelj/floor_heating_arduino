#include "circuit.h"
#include "Arduino.h"
#include "shared.h"
#include "SevSeg.h"

#define  ON 1
#define OFF 0

/*void _turn_off_all_leds(){
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

void set_display(float temp,int mode){
  // nastav tkole https://github.com/DeanIsMe/SevSeg
  char s[6];
  switch(mode){
    case 0: sprintf(s, "n%.1f",temp); break;
    case 1: sprintf(s, "V%.1f",temp); break;
    case 2: sprintf(s, "I%.1f",temp); break;
    case 3: sprintf(s, "P%.1f",temp); break;
    case 4: temp < 0 ?  sprintf(s, "c.%.1f",temp) : sprintf(s, "c%.1f",temp); break;
  }
 
  sevseg.setChars(s);
}





int read_input_down(int wanted_temp, float incremantor, float limit){
    static int old_state_down = 1;
    int new_state1 = digitalRead(BUTTON_TEMP_DOWN);

    if(old_state_down ==1 && new_state1 == 0){
        wanted_temp -= incremantor;
          Serial.println("down");

    }
    old_state_down=new_state1;
    if(wanted_temp < limit){
      wanted_temp=limit;
    }
    return wanted_temp;
    }

int read_input_up(int wanted_temp, float incremantor, float limit){
    static int old_state_up = 1;

    int new_state = digitalRead(BUTTON_TEMP_UP);
    if(old_state_up == 1 && new_state == 0){
        wanted_temp += incremantor;
                  Serial.println("up");

    }
    old_state_up = new_state;

    

     
    if(wanted_temp > limit){
      wanted_temp= limit;
    }

    return wanted_temp;
}
