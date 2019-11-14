
#ifndef _SERIAL_INTERFACE_H
#define _SERIAL_INTERFACE_H
void getFromSerial(float * serialData);
void EEPROMLoadConfig(int *temp, float *correct);
void EEPROMSaveConfig(int temp, float correct);
#endif
