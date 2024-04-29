/*
 * display.h
 *
 * Created: 14.03.2024 8:59:19
 *  Author: Tomáš
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "ADC.h"

void sendDataToLCD(unsigned char data);

void writeCharToLCD(unsigned char character);

void sendCommandToLCD(unsigned char command);

void displayStringOnLCD(char *string);


bool setLCDDisplayMode(uint8_t mode);

void initializeLCD(void);

bool clearLCD();

bool returnLCDCursorHome();

bool shiftLCDDisplayRight();

bool shiftLCDDisplayLeft();

bool shiftLCDCursorRight();

bool shiftLCDCursorLeft();

void setLCDCursorPosition(uint8_t row, uint8_t col);

int putCharLCD(char c, FILE *stream);



#endif /* DISPLAY_H_ */