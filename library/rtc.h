/*
 * #pragma once // tj. namisto #ifndef TIMER_H_ & #define TIMER_H_
 *
 * Created: 27/03/2024 17:14:03
 */
  
#pragma once // tj. namisto #ifndef TIMER_H_ & #define TIMER_H_
#include "i2c.h"

#define RTC_I2C_ADR 0x68

// Definice pinu pro generovany obdelnikovy signal (SQW/OUT)
#define SQ PD4

// Povoli modul RTC
void RTC_Enable();

// Nastavi cas RTC
void RTC_SetTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year);

// Cte cas z RTC
void RTC_ReadTime(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);

// Nastavi frekvenci ctvereckoveho signalu na vystupu RTC
void RTC_SetGen(uint8_t frequency);

// Podporne funkce pro prevod desitkovych hodnot na BCD a obracene
uint8_t RTC_DecToBcd(uint8_t val);
uint8_t RTC_BcdToDec(uint8_t val);
