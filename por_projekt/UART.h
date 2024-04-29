/*
 * UART.h
 *
 * Created: 29.02.2024 12:46:14
 *  Author: Tomáš
 */ 


#pragma once

#include <avr/io.h>
#include <stdio.h>
#include "timer.h"

#define F_CPU 16000000UL // Clock Speed



void UART_Init(uint16_t baud);

uint8_t UART_get();

void UART_write(uint8_t ch);
  
int usart_putchar_printf(char c, FILE *stream);

int UART_read_frame(uint8_t *buffer, int buffer_length);
void UART_write_array(const uint8_t *data, size_t len);
