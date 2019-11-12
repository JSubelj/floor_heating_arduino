#ifndef _SENSORS_AND_RELAYS_H
#define _SENSORS_AND_RELAYS_H

float getFloorInletTemp();
float getFloorOutletTemp();
float getFurniceTemp();

void startPump();
void stopPump();
void decreaseTemp();
void stopMixValve();
int closeMixValve();
int closeMixValveOneStep();
int setToPosition(int position);
void turnOnMixValveLight();

void increaseTemp();
void stopMixValve();
void turnOffMixValveLight();

#endif
