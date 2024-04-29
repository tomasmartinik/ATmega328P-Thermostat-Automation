/*
 * keypad.h
 *
 * Created: 19. 3. 2024 19:46:04
 *  Author: Tomáš
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>
#include <stdio.h>

#include "timer.h"

char keypadRead();

void initKeypad();

#endif /* KEYPAD_H_ */