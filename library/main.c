#include "timer.h"
#include "display.h"
#include "uart.h"
#include "rtc.h"
#include "i2c.h"
#include "menu.h"
#include "encoder.h"
#include "state_machine.h"
#include "relay.h"
#include "eeprom.h"
#include "modbus.h"


// POKUS O NASTAVENI KOMUNIKACE PO SERIOVE LINCE
//--------------------------------------------------------------------------------------
// Definice adres pro Modbus registry
//#define MB_REG_TEMPERATURE		0
//#define MB_REG_HYSTERESIS			1
//#define MB_REG_CALIBRATION		2
//#define MB_REG_SEC				3 
//#define MB_REG_MIN				4  
//#define MB_REG_HOUR				5  
//#define MB_REG_DAY				6  
//#define MB_REG_MONTH				7  
//--------------------------------------------------------------------------------------

extern float desiredTemperature;  // Deklarace, pokud již není v menu.h
extern float hysteresisSetting;

int main(void) {
	
	// POKUS O NASTAVENI KOMUNIKACE PO SERIOVE LINCE
	//--------------------------------------------------------------------------------------
	//holdingRegisters[MB_REG_TEMPERATURE] = loadFloatFromEEPROM(TEMPERATURE_REGISTER);
	//holdingRegisters[MB_REG_HYSTERESIS] = loadFloatFromEEPROM(HYSTERESIS_REGISTER);
	//holdingRegisters[MB_REG_CALIBRATION] = loadIntFromEEPROM(CALIBRATION_REGISTER);
	//DateTime dateTime = loadDateTimeFromEEPROM(DATE_TIME_REGISTER);
	//holdingRegisters[MB_REG_SEC] = dateTime.second;
	//holdingRegisters[MB_REG_MIN] = dateTime.minute;
	//holdingRegisters[MB_REG_HOUR] = dateTime.hour;
	//holdingRegisters[MB_REG_DAY] = dateTime.day;
	//holdingRegisters[MB_REG_MONTH] = dateTime.month;	
	//--------------------------------------------------------------------------------------


	I2C_Init(100000);  // Inicializace I2C s hodinovou frekvencí 100 kHz
	relay_init();
	stateMachineInit();  
	initScreen();
	float currentTemperature = get_temperature(1); 

	while (1) {
		// POKUS O KOMUNIKACI PO SERIOVE LINCE
		//--------------------------------------------------------------------------------------
		//processIncomingRequests();
		//--------------------------------------------------------------------------------------
	
		processStateMachine();	// STAVOVY AUTOMAT 
		currentTemperature = get_temperature(1);  

		// LOGIKA PRO RIZENI RELE
		if (currentTemperature > (desiredTemperature + hysteresisSetting)) {
			relay_control(0, 1);  // aktivuje ohrivani
			} else if (currentTemperature < (desiredTemperature - hysteresisSetting)) {
			relay_control(1, 0);  // aktivuje chlazeni
			} else {
			relay_control(0, 0);  // obe rele vypnuta
		}
		
		delay(30000);  
	}

	return 0;  
}
