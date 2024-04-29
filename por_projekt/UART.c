/*
 * UART.c
 *
 * Created: 29.02.2024 12:50:31
 *  Author: Tomáš
 */ 


#include "UART.h"

#define TIMEOUT_MICROSECONDS 1750 

static FILE myuart = FDEV_SETUP_STREAM(usart_putchar_printf, NULL,_FDEV_SETUP_WRITE);	

void UART_Init(uint16_t speed)
{
	uint16_t ubrr0_speed = 16000000/16/speed-1;
	
	//nastaveni rychlosti
	UBRR0H = (uint8_t)(ubrr0_speed>>8);
	UBRR0L = (uint8_t)ubrr0_speed;
	
	//asynchronni mod
	UCSR0A &= ~(1<<U2X0);	
	
	//8 bitu se bude posilat
	UCSR0C |= (1<<UCSZ01);
	UCSR0C |= (1<<UCSZ00);
	
	//1 stop bit
	UCSR0C &= ~(1<<USBS0);
	
	//povoleni vysilace a prijimace
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);	

	stdout = &myuart;
}


uint8_t UART_get()
{
	while ( !(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void UART_write(uint8_t ch)
{
	while ( !(UCSR0A & (1<< UDRE0)));
	UDR0 = ch;
	
}

int usart_putchar_printf(char c, FILE *stream) {
	
	if (c == '\n'){
		usart_putchar_printf('\r', stream);
	}
	UART_write(c);	
	return 0;
}

void UART_write_array(const uint8_t *data, size_t len)
{
	// Send array of data through UART
	for (size_t i = 0; i < len; i++) {
		UART_write(data[i]);
	}
}

int UART_read_frame(uint8_t* buffer, int bufferLength) {
	static int index = 0;  // Statická proměnná pro uchování aktuální pozice ve bufferu

	// Pokud nejsou data dostupná, okamžitě se vrať
	if (!(UCSR0A & (1<<RXC0)))
	return 0;

	// Pokud jsou data dostupná, přečti je
	while ((UCSR0A & (1<<RXC0)) && index < bufferLength) {
		buffer[index++] = UDR0;

		// Přidání kontroly na specifický konec rámu, například nový řádek
		if (buffer[index-1] == '\n') {
			int frameSize = index;
			index = 0;  // Reset indexu pro další čtení
			return frameSize;  // Vrať velikost rámu
		}
	}

	return 0;  
}
