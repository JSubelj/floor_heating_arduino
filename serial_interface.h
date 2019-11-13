
#ifndef _SERIAL_INTERFACE_H
#define _SERIAL_INTERFACE_H
void getFromSerial(int * serialData);
void EEPROMLoadConfig(struct EEPROMData *data);
void EEPROMSaveConfig(struct EEPROMData *data);
#endif