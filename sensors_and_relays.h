#ifndef _SENSORS_AND_RELAYS_H
#define _SENSORS_AND_RELAYS_H

int getFloorInletTemp();
int getFloorOutletTemp();
int getFurniceTemp();

void startPump();
void stopPump();
void decreaseTemp();
void setToPosition(int position);

#endif