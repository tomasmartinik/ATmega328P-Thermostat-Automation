#ifndef MENU_H_
#define MENU_H_

#include "state_machine.h" 
#include "display.h"
#include "rtc.h"
#include "ADC.h"
#include "UART.h"
#include "encoder.h"
#include "common_types.h"
#include "eeprom.h"
#include "modbus.h"

#define desiredTemperaturedefault 24.0
#define EEPROM_ADDRESS 0x50
#define MAX_MENU_ITEMS 5

extern ScreenState currentState;  
extern float desiredTemperature; 
extern float hysteresisSetting; 
extern int calibration;  

void initScreen();
void displayMainScreen();
void displaySettingsFirst();
void displaySettingsSecond();
void displaySettingsThird();
void freezeCurrentTime();
void switchScreen(uint8_t screenIndex);
void updateSetting(ScreenState screen, int rotation);
void updateSettingsTime(int rotation);
void switchTimeSetting();
float loadFloatFromEEPROM(uint16_t reg);
void saveFloatToEEPROM(float value, uint16_t reg);
void loadSettingsFromEEPROM();
void saveSettingsToEEPROM();
void loadSettingsFromEEPROM();
void saveIntToEEPROM(int value, uint16_t reg);
int loadIntFromEEPROM(uint16_t reg);
void saveDateTimeToEEPROM(DateTime dateTime, uint16_t reg);
DateTime loadDateTimeFromEEPROM(uint16_t reg);


#endif /* MENU_H_ */
