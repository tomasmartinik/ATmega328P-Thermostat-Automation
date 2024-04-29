/*
 * display.c
 *
 * Created: 14.03.2024 8:58:51
 *  Author: Tomáš
 */ 

#include "display.h"

void sendDataToLCD(unsigned char data){
	PORTD = (PORTD & 0x0F) | (data << 4);
	PORTB |= (1 << PINB1);
	delay(5);
	PORTB &= ~(1 << PINB1);
	delay(100);
}

void writeCharToLCD(unsigned char character){
	PORTB |= (1 << PINB0);
	sendDataToLCD(character >> 4);
	sendDataToLCD(character & 0x0F);
}

void sendCommandToLCD(unsigned char command){
	PORTB &= ~(1 << PINB0);
	sendDataToLCD(command >> 4);
	sendDataToLCD(command & 0x0F);
}

void displayStringOnLCD(char *string) {
	while (*string) {
		writeCharToLCD(*string);
		string++;
	}
}

bool setLCDDisplayMode(uint8_t mode){
	switch (mode){
		case 0:
		sendCommandToLCD(0x0c);
		delay(100);
		return true;
		case 1:
		sendCommandToLCD(0x0d);
		delay(100);
		return true;
		case 2:
		sendCommandToLCD(0x0e);
		delay(100);
		return true;
		default:
		return false;
	}
}

void initializeLCD(void){
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB1);
	PORTD &= 0;
	PORTB &= ~(1 << PINB1);
	PORTB &= ~(1 << PINB0);
	delay(20000);
	sendDataToLCD(0x03);
	delay(5000);
	sendDataToLCD(0x03);
	delay(110);
	sendDataToLCD(0x03);
	delay(3000);
	sendDataToLCD(0x02);
	delay(3000);
	sendCommandToLCD(0x28);
	delay(3000);
	setLCDDisplayMode(0);
	delay(3000);
	sendCommandToLCD(0x01);
	delay(3000);
}

bool clearLCD(){
	sendCommandToLCD(0x01);
	delay(5000);
	return true;
}

bool returnLCDCursorHome(){
	sendCommandToLCD(0x02);
	delay(5000);
	return true;
}

bool shiftLCDDisplayRight(){
	sendCommandToLCD(0x1c);
	delay(100);
	return true;
}

bool shiftLCDDisplayLeft(){
	sendCommandToLCD(0x18);
	delay(100);
	return true;
}

bool shiftLCDCursorRight(){
	sendCommandToLCD(0x14);
	delay(100);
	return true;
}

bool shiftLCDCursorLeft(){
	sendCommandToLCD(0x10);
	delay(100);
	return true;
}

void setLCDCursorPosition(uint8_t row, uint8_t col) {
	uint8_t row_offsets[] = {0x00, 0x40};
	sendCommandToLCD(0x80 | (row_offsets[row] + col));
	delay(50);
}

int putCharLCD(char c, FILE *stream) {
	if (c == '\n') putCharLCD('\r', stream);
	writeCharToLCD(c);
	return 0;
}

