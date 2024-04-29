/*
 * eeprom.h
 *
 * Created: 27.03.2024 9:55:37
 *  Author: Tomáš
 */ 



#ifndef EEPROM_H_
#define EEPROM_H_

#include "i2c.h"

#define TEMPERATURE_REGISTER 0x00
#define HYSTERESIS_REGISTER  0x20
#define CALIBRATION_REGISTER 0x40  
#define DATE_TIME_REGISTER   0x60

uint8_t i2cWriteEeprom(uint8_t addr,uint16_t reg, uint8_t *data, uint16_t len);

uint8_t i2cReadEeprom(uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len);




#endif /* EEPROM_H_ */