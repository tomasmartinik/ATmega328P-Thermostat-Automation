/*
 * encoder.c
 *
 * Created: 19. 3. 2024 18:48:44
 *  Author: Tomáš
 */ 



#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdbool.h>



#include "timer.h"

void encoderInit();

int encoderBtn();

int encoderRotation();



#endif /* ENCODER_H_ */
