/*
 * keypad.c
 *
 * Created: 19. 3. 2024 19:45:37
 *  Author: Tomáš
 */  

#include "keypad.h"

#define ROW1 (1<<PORTD2)
#define ROW2 (1<<PORTD3)
#define ROW3 (1<<PORTB3)
#define ROW4 (1<<PORTB4)
#define COL1 (1<<PORTC1)
#define COL2 (1<<PORTC2)
#define COL3 (1<<PORTC3)

int debauncing_time = 30000;

char keys[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

char keypadRead()
{
	char znak = '\0';
	
	for(int col = 0; col < 3; col++)
	{
		PORTC &= ~(1 << (col+1));
		delay(debauncing_time);
		
		if (!(PIND & (1 << PIND2)))
		{
			znak = keys[0][col];
			while(!(PIND & (1 << PIND2)))
			{
				delay(debauncing_time);
			}
			break;
		}
		else if (!(PIND & (1 << PIND3)))
		{
			znak = keys[1][col];
			while(!(PIND & (1 << PIND3)))
			{
				delay(debauncing_time);
			}
			break;
		}
		else if (!(PINB & (1 << PINB3)))
		{
			znak = keys[2][col];
			while(!(PINB & (1 << PINB3)))
			{
				delay(debauncing_time);
			}
			break;
		}
		else if (!(PINB & (1 << PINB4)))
		{
			znak = keys[3][col];
			while(!(PINB & (1 << PINB4)))
			{
				delay(debauncing_time);
			}
			break;
		}
		
		PORTC |= (1 << (col+1));
	}
	return znak;
}

void initKeypad() {
	DDRD &= ~(ROW1 | ROW2);
	PORTD |= (ROW1 | ROW2);

	DDRB &= ~(ROW3 | ROW4);
	PORTB |= (ROW3 | ROW4);

	DDRC |= (COL1 | COL2 | COL3);
	PORTC |= (COL1 | COL2 | COL3);
}
