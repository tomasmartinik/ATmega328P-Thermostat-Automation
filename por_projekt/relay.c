/*
 * relay.c
 *
 * Created: 28.04.2024 3:57:25
 *  Author: Tomáš
 */ 

#include "relay.h"


// Časové konstanty pro omezení spínání relé
#define RELAY_DELAY_MS 1000000

uint32_t last_toggle = 0;

void relay_init(void) {
	RELAY_DDR |= (1 << RELAY_PIN_COOLING) | (1 << RELAY_PIN_HEATING); // Nastavení pinů jako výstupní
	RELAY_PORT |= (1 << RELAY_PIN_COOLING) | (1 << RELAY_PIN_HEATING); // Deaktivace obou relé (nastavení pinů na vysokou logickou úroveň)
}


void relay_control(bool cooling, bool heating) {
	static uint32_t last_toggle_cooling = 0;
	static uint32_t last_toggle_heating = 0;
	uint32_t current_time = getTime();

	// Kontrola a spínání relé pro ohřívání
	if (heating && !cooling && (current_time - last_toggle_heating >= RELAY_DELAY_MS)) {
		RELAY_PORT &= ~(1 << RELAY_PIN_COOLING); // Zapne relé pro chlazení (aktivní na nízké úrovni)
		RELAY_PORT |= (1 << RELAY_PIN_HEATING);  // Vypne relé pro ohřívání
		last_toggle_heating = current_time;      // Uložení času posledního přepnutí pro chlazení
	}

	// Kontrola a spínání relé pro chlazení
	if (cooling && !heating && (current_time - last_toggle_cooling >= RELAY_DELAY_MS)) {
		RELAY_PORT &= ~(1 << RELAY_PIN_HEATING); // Zapne relé pro ohřívání (aktivní na nízké úrovni)
		RELAY_PORT |= (1 << RELAY_PIN_COOLING);  // Vypne relé pro chlazení
		last_toggle_cooling = current_time;      // Uložení času posledního přepnutí pro ohřívání
	}

	// Vypnutí obou relé, pokud nejsou potřeba
	if (!cooling && !heating) {
		RELAY_PORT |= (1 << RELAY_PIN_COOLING) | (1 << RELAY_PIN_HEATING); // Vypne obě relé
	}
}


extern uint32_t getTime(void); // Předpokládá se, že funkce getTime() je definována někde jinde

