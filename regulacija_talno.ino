// temp senzor: https://randomnerdtutorials.com/arduino-lm35-lm335-lm34-temperatur2000e-sensor/
#include "mixed_valve_regulation.h"

#include "Arduino.h"
#include "circuit.h"
#include "shared.h"
#include "configuration.h"
#include "sensors_and_relays.h"
#include "serial_interface.h"
#include "interface.h"
#include <EEPROM.h>
#define TICK_DURATION_MS 50
#include "SevSeg.h"


// Taski:
//  mer temperaturo
//  spreme regulacijo
//  prdob podatke iz 

// save_indicator must be 1488

SevSeg sevseg; //Instantiate a seven segment object

//int control(int a, int b, int c, int d){return 0;}
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

    pinMode(CHANGE_CORRECTION_PIN_GROUND, OUTPUT);
    pinMode(CHANGE_CORRECTION_PIN_PULLUP, INPUT_PULLUP);
    digitalWrite(CHANGE_CORRECTION_PIN_GROUND, 0);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F1, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(DP, OUTPUT);


    // nastav tkole https://github.com/DeanIsMe/SevSeg
    byte numDigits = 3;
    byte digitPins[] = {INDICATOR_7_DRAIN, LEFT_7_DRAIN, RIGHT_7_DRAIN};
    byte segmentPins[] = {A, B, C, D, E, F1, G, DP};
    bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
    byte hardwareConfig = COMMON_CATHODE; // See README.md for options
    bool updateWithDelays = true; // Default 'false' is Recommended
    bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
    bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
    updateWithDelays, leadingZeros, disableDecPoint);
    

    digitalWrite(INDICATOR_7_DRAIN,0);
    //digitalWrite(LEFT_7_DRAIN,0);
    //digitalWrite(RIGHT_7_DRAIN,0);
    digitalWrite(49,1);


    pinMode(INDICATOR_LED, OUTPUT);
    pinMode(BUTTON_TEMP_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_TEMP_UP, INPUT_PULLUP);

    digitalWrite(RELAY_DECREASE_TEMP, 1);
    digitalWrite(RELAY_INCREASE_TEMP, 1);
    digitalWrite(RELAY_PUMP, 1);
    EEPROM.get(0, temp_wanted);
    EEPROM.get(sizeof(int),temp_correction);

    sevseg.setChars("abc");
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
float temp_floor_inlet;    
float temp_floor_outlet;   
float temp_furnice;
float temp_floor_inlet_sum;    
float temp_floor_outlet_sum;   
float temp_furnice_sum;
unsigned long ticks_millis = millis();
int ticks = 0;
int temp_reading =0;
void loop(){
  unsigned int starttime=millis();
      sevseg.refreshDisplay();
        //    Serial.print(millis()-starttime);

      //Serial.println("Took refreshing:");
      control1(temp_wanted, temp_floor_inlet, temp_floor_outlet, temp_furnice);
      
  // 3600mV pri 100°C 373K

            static bool exec_250 = false;

    if(millis() - ticks_millis >= TICK_DURATION_MS){
        ticks++;
        ticks_millis = millis();
        exec_250 = true;
    }
    digitalWrite(INDICATOR_LED,!digitalRead(CHANGE_CORRECTION_PIN_PULLUP));
    

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
          temp_floor_inlet =  temp_floor_inlet_sum/readings_taken_in;
          temp_floor_outlet = temp_floor_outlet_sum/readings_taken_out;
          temp_furnice = temp_furnice_sum/readings_taken_furnice;
          readings_taken_in = 0;
          readings_taken_out = 0;
          readings_taken_furnice = 0;
          start_time_temp_reading = millis();
          temp_floor_inlet_sum = 0;
          temp_floor_outlet_sum = 0;
          temp_furnice_sum = 0;
  

         
          if(temp_floor_inlet >= MAX_INLET_TEMP){
              TOO_HOT = true;
              stopPump();
          }
          
        }
        temp_reading++;
    }

    // exec every 250 ms
    static int current_mode = 0;
        static unsigned long current_time = millis();

    if(ticks % (3000/TICK_DURATION_MS) == 0 && exec_250){
      exec_250 = false;
     

        
      Serial.print("passed ");
      Serial.println(millis()-current_time);
      current_time=millis();
      
      current_mode++;
      current_mode %=5;
      switch(current_mode){
          case 0: set_display(temp_wanted,current_mode);break;
          case 1: set_display(temp_floor_inlet,current_mode);break;
          case 2: set_display(temp_floor_outlet,current_mode);break;
          case 3: set_display(temp_furnice,current_mode);break;
          case 4: set_display(temp_correction,current_mode);break;
      }    
      }

        
   // exec every 100 ms
    if(ticks % (100/TICK_DURATION_MS) == 0){
        if(digitalRead(CHANGE_CORRECTION_PIN_PULLUP)){
            int old_temp = temp_wanted;
            temp_wanted = read_input_down(temp_wanted,5,10);
            temp_wanted = read_input_up(temp_wanted,5,55);
            if(old_temp!= temp_wanted){
                Serial.println("stored");
                EEPROMSaveConfig(temp_wanted,temp_correction);
                set_display(temp_wanted,0);         
            }
        }else{
            float old_temp_correction = temp_correction;
            temp_correction = read_input_down(temp_correction,0.5,-9.5);
            temp_correction = read_input_up(temp_correction,0.5,9.5);
            if(old_temp_correction != temp_correction){
                Serial.println("stored");
                EEPROMSaveConfig(temp_wanted,temp_correction);
                set_display(temp_correction,4);   
            }
        }
        
    }
    
    if(ticks % (10000/TICK_DURATION_MS) == 0){
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



    float serialData[2]= {};
    getFromSerial(serialData);
    switch((int)serialData[0]){
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
