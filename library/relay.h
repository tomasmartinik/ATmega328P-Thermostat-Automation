#ifndef RELAY_H_
#define RELAY_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "timer.h"


// Definice pinů pro relé
#define RELAY_DDR DDRC
#define RELAY_PORT PORTC
#define RELAY_PIN_COOLING PC2
#define RELAY_PIN_HEATING PC3

// Funkce pro inicializaci relé
void relay_init(void);

// Funkce pro ovládání relé
void relay_control(bool cooling, bool heating);

#endif /* RELAY_H_ */