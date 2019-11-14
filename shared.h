#ifndef SHAREFILE_INCLUDED
#define SHAREFILE_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "SevSeg.h"

extern float wanted_position;
extern int current_position_mix_valve;
extern int temp_correction;
extern SevSeg sevseg; //Instantiate a seven segment object

#ifdef __cplusplus
}
#endif

#endif

