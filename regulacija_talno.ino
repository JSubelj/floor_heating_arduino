// temp senzor: https://randomnerdtutorials.com/arduino-lm35-lm335-lm34-temperature-sensor/
#include "Arduino.h"
#include "circuit.h"
#include "shared.h"
#include "configuration.h"
#include "sensors_and_relays.h"
#include "mixed_valve_regulation.h"
#include "serial_interface.h"
#include "interface.h"
#include <EEPROM.h>
#define TICK_DURATION_MS 50

// Taski:
//  mer temperaturo
//  spreme regulacijo
//  prdob podatke iz 

// save_indicator must be 1488
struct EEPROMData {
    int wanted_temp;
    int sensor_correction;
};

int temp_wanted = INITIAL_TEMPERATURE;

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

    for(int i=0; all_LEDs[i] != -1; i++){
        pinMode(all_LEDs[i], OUTPUT);
    }
    pinMode(INDICATOR_LED, OUTPUT);
    pinMode(BUTTON_TEMP_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_TEMP_UP, INPUT_PULLUP);

    digitalWrite(RELAY_DECREASE_TEMP, 1);
    digitalWrite(RELAY_INCREASE_TEMP, 1);
    digitalWrite(RELAY_PUMP, 1);
    digitalWrite(MIXER_VALVE_LED, 0);
    EEPROM.get(0, temp_wanted);
    EEPROM.get(sizeof(int),temp_correction);
    
    
}




int TOO_HOT = false;
unsigned long ten_minutes_timer = millis();



unsigned long start_time = millis();
unsigned long start_time_temp_reading = millis();
unsigned long timing_for_adc = millis();
unsigned long start_time_display_leds = millis();
int select_adc_channel = 0;
int readings_taken_in = 0;
int readings_taken_out = 0;
int readings_taken_furnice = 0;
int temp_floor_inlet;    
int temp_floor_outlet;   
int temp_furnice;
float temp_floor_inlet_sum;    
float temp_floor_outlet_sum;   
float temp_furnice_sum;
unsigned long ticks_millis = millis();
int ticks = 0;
int temp_reading =0;
void loop(){
  // 3600mV pri 100°C 373K
  /*#define sensorPin A0

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
  delay(1000);*/
    
    if(millis() - ticks_millis >= TICK_DURATION_MS){
        ticks++;
        ticks_millis = millis();
    }
    

    // exec every 50 ms
    if(ticks % (50/TICK_DURATION_MS) == 0){
        // +++++++++input read+++++++++
        // ---------input read---------

        // +++++++++temp read+++++++++
        switch(select_adc_channel){
            case 0:{
                temp_floor_inlet_sum +=  getFloorInletTemp();
                readings_taken_in++;
                break;
            }
            case 1:{
                temp_floor_outlet_sum += getFloorOutletTemp();
                readings_taken_out++;
                break;
            }
            case 2:{
                temp_furnice_sum +=      getFurniceTemp();
                readings_taken_furnice++;
                break;
            }
        }
        select_adc_channel++;
        select_adc_channel%=3;
        // -------temp read-----------

        if(temp_reading >=20){
          temp_reading = 0;
          temp_floor_inlet =  round(temp_floor_inlet_sum/readings_taken_in);
          temp_floor_outlet = round(temp_floor_outlet_sum/readings_taken_out);
          temp_furnice = round(temp_furnice_sum/readings_taken_furnice);
          readings_taken_in = 0;
          readings_taken_out = 0;
          readings_taken_furnice = 0;
          start_time_temp_reading = millis();
          temp_floor_inlet_sum = 0;
          temp_floor_outlet_sum = 0;
          temp_furnice_sum = 0;
  

          if(temp_floor_inlet > temp_wanted-3 && temp_floor_inlet < temp_wanted+3){
                turnOnMixValveLight();
          }else{
            turnOffMixValveLight();
          }
          if(temp_floor_inlet >= MAX_INLET_TEMP){
              TOO_HOT = true;
              stopPump();
          }
          
        }
        temp_reading++;
    }

    // exec every 250 ms
    static int current_mode = 0;
    if(ticks % (250/TICK_DURATION_MS) == 0){
      switch(current_mode){
        case 0: set_leds(temp_wanted,1);break;
        /*case 1: set_leds(temp_floor_inlet,2);break;
        case 2: set_leds(temp_floor_outlet,3);break;
        case 3: set_leds(temp_floor_outlet,4);break;*/

      }
                

    }    


        
    // exec every 100 ms
    if(ticks % (100/TICK_DURATION_MS) == 0){
        int old_temp = temp_wanted;
        temp_wanted = read_input_down(temp_wanted);
        temp_wanted = read_input_up(temp_wanted);
        if(old_temp!= temp_wanted){
          Serial.println("stored");
          EEPROMSaveConfig(temp_wanted,temp_correction);
          
          EEPROMLoadConfig(&temp_wanted,&temp_correction);
        
        }

        
    }
    
    if(ticks % (1000/TICK_DURATION_MS) == 0){
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
        Serial.print("Wanted temperature: ");
        Serial.print(temp_wanted);
        Serial.print("; Temperature floor inlet: ");
        Serial.print(temp_floor_inlet);
        Serial.print("; Temperature floor outlet: ");
        Serial.print(temp_floor_outlet);
        Serial.print("; Temperature furnice: ");
        Serial.println(temp_furnice);
        start_time = millis();
    }


    // exec all the time

    control(temp_wanted, temp_floor_inlet, temp_floor_outlet, temp_furnice);


    int serialData[2]= {};
    getFromSerial(serialData);
    switch(serialData[0]){
        case 1:{
            temp_wanted = serialData[1];
            break;
        }
        case 2:{
            temp_correction = serialData[1];
            break;
        }
        default: break;      
    }
    if(serialData[0]){
        
         EEPROMSaveConfig(temp_wanted,temp_correction);
    }

}
