/*
 * common_types.h
 *
 * Created: 28.04.2024 1:56:21
 *  Author: Tomáš
 */ 


#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_


typedef enum {
	MAIN_SCREEN = 0,
	SETTINGS_DATE,
	SETTINGS_HYSTERESIS,
	SETTINGS_CALIBRATION,
	NUM_SCREENS
} ScreenState;

typedef enum {
	SET_DAY,
	SET_MONTH,
	SET_HOUR,
	SET_MINUTE,
	SET_SECOND,
	NUM_TIME_SETTINGS  // Pomáhá s cyklickým přepínáním
} TimeSettingType;

typedef struct {
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} DateTime;


#endif /* COMMON_TYPES_H_ */