/*
 * menu.c
 *
 * Created: 21.04.2024 11:56:45
 *  Author: Tomáš
 */ 

#include <string.h> 
#include "menu.h"

static FILE display = FDEV_SETUP_STREAM(putCharLCD, NULL,_FDEV_SETUP_WRITE);    // Redirecting stdout to LCD

uint8_t init_sec, init_min, init_hour, init_day, init_date, init_month, init_year;
uint8_t frozen_sec, frozen_min, frozen_hour, frozen_day, frozen_date, frozen_month, frozen_year;
uint8_t sec, min, hour, date, day, month, year;
float desiredTemperature; 
float hysteresisSetting = 1;  
extern int calibration;
int enc_val = 0;
uint8_t currentMenuItem = 0;

void initScreen() {
	adc_init();
	timerInit();
	modbus_init(38400, 0, 1);
	I2C_Init(38400);
	loadSettingsFromEEPROM();
	RTC_Enable(); 
	RTC_SetGen(1);
	initializeLCD();
	encoderInit();
	RTC_SetTime(init_sec, init_min, init_hour, init_day, init_day, init_month, init_year);
	stdout = &display;
}

void freezeCurrentTime() {
	RTC_ReadTime(&frozen_sec, &frozen_min, &frozen_hour, &frozen_day, &frozen_day, &frozen_month, &frozen_year);
}

void updateSetting(ScreenState screen, int rotation) {
	switch(screen) {
		case MAIN_SCREEN:
		desiredTemperature += rotation * 0.5;  // Změna požadované teploty
		saveFloatToEEPROM(desiredTemperature, TEMPERATURE_REGISTER);
		break;
		case SETTINGS_HYSTERESIS:
		hysteresisSetting += rotation * 0.5;  // Změna nastavení hystereze
		saveFloatToEEPROM(hysteresisSetting, HYSTERESIS_REGISTER);
		break;
		case SETTINGS_CALIBRATION:
		calibration += rotation * 10;   // Změna nastavení kalibrace
		saveIntToEEPROM(calibration, CALIBRATION_REGISTER);
		break;
		case SETTINGS_DATE:
		// Zde není potřeba žádná akce, protože SETTINGS_DATE má vlastní funkci update
		break;
		default:
		// Neznámá obrazovka
		break;
	}
	//saveSettingsToEEPROM();
}

void switchTimeSetting() {
	if (currentSetting == SET_SECOND) {
		currentState = (currentState + 1) % NUM_SCREENS;  // Přechod na další obrazovku
		currentSetting = SET_DAY;  // Reset na hodiny pro další použití nastavení času
		} else {
		currentSetting = (TimeSettingType)((currentSetting + 1) % NUM_TIME_SETTINGS);
	}
}



void displayMainScreen() {
	enc_val = encoderRotation();
	//desiredTemperature = encoderRotation();
	float currentTemperature = get_temperature(1);  // Aktuální teplota
	//float desiredTemperature = desiredTemperaturedefault + (enc_val*0.5);
	uint8_t sec, min, hour, day, date, month, year;

	// Simulované načtení dat z RTC
	RTC_ReadTime(&sec, &min, &hour, &day, &day, &month, &year);

	// Nastavení kurzoru na začátek prvního řádku displeje
	setLCDCursorPosition(0, 0);
	//fprintf(stdout, "Hodnota: %d", enc_val);

	// Zobrazení teplot s přidaným symbolem stupně
	fprintf(stdout, "%.1f%cC => %.1f%cC", currentTemperature, 223, desiredTemperature, 223);


	// Nastavení kurzoru na začátek druhého řádku displeje
	setLCDCursorPosition(1, 0);
	// Zobrazení data a času včetně sekund
	fprintf(stdout, "%02d/%02d %02d:%02d:%02d", day, month, hour, min, sec);
	
	delay(300000);

}

void displaySettingsFirst() {
	clearLCD();
	setLCDCursorPosition(0, 0);
	fprintf(stdout, "Hystereze:");
	setLCDCursorPosition(1, 0);
	fprintf(stdout, "%.1f", hysteresisSetting);
	// další logika pro zobrazení nastavení hystereze
}

void displaySettingsSecond() {
	clearLCD();
	setLCDCursorPosition(0, 0);
	fprintf(stdout, "Datum a cas:");
	setLCDCursorPosition(1, 0);
	fprintf(stdout, "%02d/%02d %02d:%02d:%02d", frozen_day, frozen_month, frozen_hour, frozen_min, frozen_sec);
	// další logika pro zobrazení nastavení data a času
}

void displaySettingsThird() {
	clearLCD();
	setLCDCursorPosition(0, 0);
	fprintf(stdout, "Kalibrace");
	setLCDCursorPosition(1, 0);
	fprintf(stdout, "%d", calibration);
	// další logika pro zobrazení kalibrace
}


void updateSettingsTime(int rotation) {
	DateTime dateTime;

	// Fetch current time to modify
	RTC_ReadTime(&dateTime.second, &dateTime.minute, &dateTime.hour,
	&dateTime.day, &dateTime.day, &dateTime.month, &dateTime.year);

	switch (currentSetting) {
		case SET_DAY:
		dateTime.day += rotation;
		if (dateTime.day > 31) {
			dateTime.day = 1;
			} else if (dateTime.day < 1) {
			dateTime.day = 31;
		}
		frozen_day = dateTime.day;
		break;
		case SET_MONTH:
		dateTime.month += rotation;
		if (dateTime.month > 12) {
			dateTime.month = 1;
			} else if (dateTime.month < 1) {
			dateTime.month = 12;
		}
		frozen_month = dateTime.month;
		break;
		case SET_HOUR:
		dateTime.hour = (dateTime.hour + rotation + 24) % 24;
		frozen_hour = dateTime.hour;
		break;
		case SET_MINUTE:
		dateTime.minute = (dateTime.minute + rotation + 60) % 60;
		frozen_min = dateTime.minute;
		break;
		case SET_SECOND:
		dateTime.second = (dateTime.second + rotation + 60) % 60;
		frozen_sec = dateTime.second;
		break;
	}

	RTC_SetTime(dateTime.second, dateTime.minute, dateTime.hour,
	1, dateTime.day, dateTime.month, dateTime.year);

	saveDateTimeToEEPROM(dateTime, DATE_TIME_REGISTER);
}


float loadFloatFromEEPROM(uint16_t reg) {
    uint8_t data[4];
    float value;

    uint8_t result = i2cReadEeprom(EEPROM_ADDRESS, reg, data, sizeof(float));
    if (result == I2C_OK) {
        memcpy(&value, data, sizeof(float));
        return value;
    } else {
        return -1.0f; 
    }
}


// Pomocná funkce pro ukládání hodnot do EEPROM
void saveFloatToEEPROM(float value, uint16_t reg) {
    uint8_t data[4];
    memcpy(data, &value, sizeof(float));

    uint8_t result = i2cWriteEeprom(EEPROM_ADDRESS, reg, data, sizeof(float));
    if (result != I2C_OK) {}
}


void loadSettingsFromEEPROM() {
	desiredTemperature = loadFloatFromEEPROM(TEMPERATURE_REGISTER);
	hysteresisSetting = loadFloatFromEEPROM(HYSTERESIS_REGISTER);
	calibration = loadIntFromEEPROM(CALIBRATION_REGISTER);  
	DateTime dateAndTime = loadDateTimeFromEEPROM(DATE_TIME_REGISTER);  

	init_sec = dateAndTime.second;
	init_min = dateAndTime.minute;
	init_hour = dateAndTime.hour;
	init_day = dateAndTime.day;
	init_month = dateAndTime.month;
	init_year = dateAndTime.year;
}

void saveSettingsToEEPROM() {
    saveFloatToEEPROM(desiredTemperature, TEMPERATURE_REGISTER);
    saveFloatToEEPROM(hysteresisSetting, HYSTERESIS_REGISTER);
    saveIntToEEPROM(calibration, CALIBRATION_REGISTER);
}


void saveIntToEEPROM(int value, uint16_t reg) {
    uint8_t data[sizeof(int)];  // Vytvoření bajtového pole pro int
    memcpy(data, &value, sizeof(int));  // Kopírování int do bajtového pole

    uint8_t result = i2cWriteEeprom(EEPROM_ADDRESS, reg, data, sizeof(int));  // Zápis bajtového pole do EEPROM
    if (result != I2C_OK) {
    }
}


int loadIntFromEEPROM(uint16_t reg) {
    uint8_t data[sizeof(int)];
    int value;

    uint8_t result = i2cReadEeprom(EEPROM_ADDRESS, reg, data, sizeof(int));  // Čtení bajtového pole z EEPROM
    if (result == I2C_OK) {
        memcpy(&value, data, sizeof(int));  // Převedení bajtového pole zpět na int
        return value;
    } else {
        return -1;  
    }
}

void saveDateTimeToEEPROM(DateTime dateTime, uint16_t reg) {
	uint8_t data[sizeof(DateTime)];
	memcpy(data, &dateTime, sizeof(DateTime));

	uint8_t result = i2cWriteEeprom(EEPROM_ADDRESS, reg, data, sizeof(DateTime));
	if (result != I2C_OK) {}
}

DateTime loadDateTimeFromEEPROM(uint16_t reg) {
	uint8_t data[sizeof(DateTime)];
	DateTime dateTime;

	uint8_t result = i2cReadEeprom(EEPROM_ADDRESS, reg, data, sizeof(DateTime));
	if (result == I2C_OK) {
		memcpy(&dateTime, data, sizeof(DateTime));
		return dateTime;
		} else {
		DateTime errorDate = {0};
		return errorDate;
	}
}
