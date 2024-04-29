/*
 * encoder.c
 *
 * Created: 19. 3. 2024 19:13:53
 *  Author: Tomáš
 */  

#include "encoder.h"

volatile int encoderValue = 0;
volatile int buttonCounter = 0;
int count = 0;

uint8_t state = 0;
uint8_t lastState = 0;
int lastValueEnc = 0;
int lastValueBtn = 0;
int debaucingTime = 20000;

volatile bool buttonPressed = false;
volatile uint32_t buttonPressTime = 0;
volatile uint32_t buttonReleaseTime = 0;


ISR(INT0_vect) {
	EIMSK &= ~(1 << INT0);  

	if(!(PIND & (1 << PIND3))) {
		encoderValue++;
		} else {
		encoderValue--;
	}

	
}

ISR(PCINT0_vect) {
	PCICR &= ~(1 << PCIE0);
	buttonCounter++;
}

int encoderBtn() {
	if(buttonCounter != lastValueBtn) {
		
		state = (PINB & (1<<DDB3));
		if(state == lastState) {
			lastState = state;
			count++;
		}
		
		lastValueBtn = buttonCounter;
		delay(debaucingTime);
		PCIFR |= (1 << PCIF0);
		PCICR |= (1 << PCIE0);
	}
	return count;
}


int encoderRotation() {
	if(encoderValue != lastValueEnc) {
		
		lastValueEnc = encoderValue;
		delay(debaucingTime);
		EIFR |= (1 << INTF0);
		EIMSK |= (1 << INT0);
	}
	return encoderValue;
}

void encoderInit() {
	DDRD &= ~((1 << DDD2) | (1 << DDD3));
	DDRB &= ~(1 << DDB3);
	PORTB |= (1 << DDB3);
	
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT3);
	
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
}
