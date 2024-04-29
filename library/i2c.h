/*
 * I2C.h
 *
 * Created: 27/03/2024 17:51:24
 */ 

#include "timer.h"
#include <stdbool.h>
#pragma once // tj. namisto #ifndef TIMER_H_ & #define TIMER_H_

#define CPU_CLOCK 16000000UL // Frekvence CPU

// Definice pro stavové kódy I2C
#define I2C_OK      0
#define I2C_ERROR   1

// I2C TWSR Status kody
#define I2C_START       0x08 // Start podminka byla odeslana
#define I2C_REP_START   0x10 // Opakovana Start podminka byla odeslana
#define I2C_MT_SLA_ACK  0x18 // Slave adresa byla odeslana a ACK bylo obdrzeno
#define I2C_MT_DATA_ACK 0x28 // Data byla odeslana a ACK bylo obdrzeno
#define I2C_MR_SLA_ACK  0x40 // Slave adresa byla odeslana a ACK bylo obdrzeno (rezim cteni)
#define I2C_MR_DATA_ACK 0x50 // Data byla obdrzena a ACK bylo odeslano (rezim cteni)
#define I2C_MR_DATA_NACK 0x58 // Data byla obdrzena a NACK bylo odeslano (rezim cteni)
#define I2C_TWSTA_ACK   0x08 // TWI Start Condition successfully transmitted
#define I2C_TWSTO_ACK   0x10 // TWI Stop Condition successfully transmitted
#define I2C_TWEA_ACK    0x30 // TWI Address + data transmitted, ACK received
#define I2C_TWEA_NACK   0x38 // TWI data transmitted, NACK received
#define I2C_STATUS_MASK 0xF8 // Maska pro statusove bity TWSR

void I2C_Init(uint32_t clockFrequency);
bool I2C_Start();
bool I2C_RepStart();
void I2C_Stop();
bool I2C_Read(uint8_t *data, bool ack);
bool I2C_WAddr(uint8_t slaveAdr7bit, bool readWrite);
bool I2C_WData(uint8_t data);
