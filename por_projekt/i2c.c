/*
 * I2C.c
 * Created: 27/03/2024 17:51:02
 */ 

#include "I2C.h"
#include <avr/io.h>

#define CPU_CLOCK 16000000UL  

// I2C TWSR Status codes
#define START           0x08
#define REPEATED_START  0x10
#define MT_SLA_ACK      0x18
#define MT_DATA_ACK     0x28
#define MR_SLA_ACK      0x40
#define MR_DATA_ACK     0x50
#define MR_DATA_NACK    0x58

void I2C_Init(uint32_t clockFrequency) {
    TWSR = 0; 
    TWBR = ((CPU_CLOCK / clockFrequency) - 16) / 2; 
    TWCR = (1 << TWEN); 
}

bool I2C_Start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Odeslání start podmínky
    while (!(TWCR & (1 << TWINT))); // Čekání na nastavení TWINT flagu (indikuje dokončení operace)
    
    return ((TWSR & 0xF8) == START); // Kontrola, zda byla start podmínka úspěšně odeslána
}

bool I2C_RepStart() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Odeslání opakované start podmínky
    while (!(TWCR & (1 << TWINT))); // Čekání na nastavení TWINT flagu
    
    return ((TWSR & 0xF8) == REPEATED_START); // Kontrola, zda byla opakovaná start podmínka úspěšně odeslána
}

void I2C_Stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Odeslání stop podmínky
    while (TWCR & (1 << TWSTO)); // Čekání na dokončení stop podmínky
}

bool I2C_Read(uint8_t *data, bool ack) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0); // Nastavení pro příjem dat s ACK/NACK
    while (!(TWCR & (1 << TWINT))); // Čekání na nastavení TWINT flagu

    *data = TWDR; // Přečtení přijatých dat
    return ((TWSR & 0xF8) == (ack ? MR_DATA_ACK : MR_DATA_NACK)); // Kontrola úspěšného příjmu dat
}

bool I2C_WAddr(uint8_t slaveAdr7bit, bool readWrite) {
    TWDR = (slaveAdr7bit << 1) | readWrite; // Nastavení adresy zařízení a bitu pro čtení/zápis
    TWCR = (1 << TWINT) | (1 << TWEN); // Odeslání adresy zařízení
    while (!(TWCR & (1 << TWINT))); // Čekání na nastavení TWINT flagu

    return ((TWSR & 0xF8) == (readWrite ? MR_SLA_ACK : MT_SLA_ACK)); // Kontrola, zda byla adresa zařízení úspěšně přijata
}

bool I2C_WData(uint8_t data) {
    TWDR = data; // Nastavení dat k odeslání
    TWCR = (1 << TWINT) | (1 << TWEN); // Odeslání dat
    while (!(TWCR & (1 << TWINT))); // Čekání na nastavení TWINT flagu

    return ((TWSR & 0xF8) == MT_DATA_ACK); // Kontrola úspěšného odeslání dat
}
