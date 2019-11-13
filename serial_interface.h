
#ifndef _SERIAL_INTERFACE_H
#define _SERIAL_INTERFACE_H
void getFromSerial(int * serialData);
void EEPROMLoadConfig(int *temp, int *correct);
void EEPROMSaveConfig(int temp, int correct);
#endif
