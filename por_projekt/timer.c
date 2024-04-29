/*
 * timer.c
 *
 * Created: 28.02.2024 11:20:24
 *  Author: Tomáš
 */ 

#include "timer.h"

// Definice makra No Operation pro vytvareni kratsich zpozdeni
#define NOP() asm volatile ("nop");

// Globální proměnná pro počet přetečení časovače
volatile uint16_t overflowCount = 0;
// Globální proměnná pro uchování počátečního času
uint32_t startTime = 0;

// Funkce přerušení pro přetečení časovače 1
ISR(TIMER1_OVF_vect){
	overflowCount++; // Inkrementace počtu přetečení
}

// Inicializace časovače
void timerInit(void){
	TCNT1 = 0;		// Vynulování časovače
	TIFR1 = (1<<TOV1) | (1<<OCF1A);	// Smazání vlajek přerušení
	TCCR1B |= (1<<CS11); // Zapnutí časovače, předdělička nastavena na 8
	TIMSK1 |= (1<<TOIE1); // Povolení přerušení od přetečení
	sei(); // Povolení všech přerušení (globálně)
}

// Funkce pro získání aktuálního času
uint32_t getTime(void){
	return (((uint32_t)overflowCount * 0xFFFF)+TCNT1)>>1; // Výpočet času pomocí počtu přetečení a aktuální hodnoty časovače, posunutí o 1 bit vpravo (dělení dvěma)
}

// Funkce pro aktivní čekání (busy delay) v mikrosekundách
void busyDelay(uint32_t us){
	us -= 2; // Pro režii potřebujeme 2 us, odečteme z celku
	
	// 7x NOP, aby celková režie byla 2 us
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	
	// Pro každé zbývající us přidáme 6x NOP, protože for trvá 0.6250 us a do 1 us potřebujeme 6x NOP
	for (;us!=0;--us)
	{
		NOP();NOP();NOP();NOP();NOP();NOP();
	}
}

// Funkce pro zpoždění na základě časovače
void delay(uint32_t us){
	startTime = getTime() + us; // Nastavíme startovní čas
	while (getTime() < startTime); // Čekáme, až uplyne nastavený čas
}