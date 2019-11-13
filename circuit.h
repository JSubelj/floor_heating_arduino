#ifndef _CIRCUIT_H
#define _CIRCUIT_H

#define TEMP_FURNICE A0 // before mixed valve     A0
#define TEMP_FLOOR_INLET A1 // after mixed valve  A1
#define TEMP_FLOOR_OUTLET A2                  //  A2
#define RELAY_DECREASE_TEMP 10
#define RELAY_INCREASE_TEMP 11
#define RELAY_PUMP 12

#define MIXER_VALVE_LED 13

#define LED_15_C 52
#define LED_20_C 50
#define LED_25_C 48
#define LED_30_C 46
#define LED_35_C 44
#define LED_40_C 42
#define LED_45_C 40
#define LED_50_C 38
#define LED_55_C 36
#define INDICATOR_LED 34

#define BUTTON_TEMP_UP 32
#define BUTTON_TEMP_DOWN 30

// const char a_LEDs_10_C[] = { -1 };
// const char a_LEDs_15_C[] = { LED_15_C, -1 };
// const char a_LEDs_20_C[] = { LED_15_C, LED_20_C, -1 };
// const char a_LEDs_25_C[] = { LED_15_C, LED_20_C, LED_25_C, -1 };
// const char a_LEDs_30_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, -1 };
// const char a_LEDs_35_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, -1 };
// const char a_LEDs_40_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, LED_40_C, -1 };
// const char a_LEDs_45_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, LED_40_C, LED_45_C, -1 };
// const char a_LEDs_50_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, LED_40_C, LED_45_C, LED_50_C -1 };
// const char a_LEDs_55_C[] = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, LED_40_C, LED_45_C, LED_50_C, LED_55_C, -1 };
const char all_LEDs[]    = { LED_15_C, LED_20_C, LED_25_C, LED_30_C, LED_35_C, LED_40_C, LED_45_C, LED_50_C, LED_55_C, -1 };













#endif
