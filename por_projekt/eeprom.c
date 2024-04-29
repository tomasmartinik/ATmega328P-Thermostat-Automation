/*
 * eeprom.c
 *
 * Created: 27.03.2024 9:52:01
 *  Author: Tom�
 */ 

#include "eeprom.h"


uint8_t i2cReadEeprom(uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len) {
	uint16_t i = 0;
	uint8_t firstReg, secondReg;

	if (len > 32) {
		return 0xFF;  // Omezen� d�lky na 32 bajt�
	}

	if (!I2C_Start()) {
		I2C_Stop();
		return I2C_ERROR;
	}

	if (!I2C_WAddr(addr, false)) {  // Z�pisov� operace, false = write mode
		I2C_Stop();
		return I2C_ERROR;
	}

	firstReg = (reg >> 8) & 0xFF;
	secondReg = reg & 0xFF;

	if (!I2C_WData(firstReg) || !I2C_WData(secondReg)) {
		I2C_Stop();
		return I2C_ERROR;
	}

	if (!I2C_RepStart()) {
		I2C_Stop();
		return I2C_ERROR;
	}

	if (!I2C_WAddr(addr, true)) {  // �tec� operace, true = read mode
		I2C_Stop();
		return I2C_ERROR;
	}

	for (i = 0; i < len - 1; i++) {
		if (!I2C_Read(&data[i], true)) {  // Ack pro v�echny krom� posledn�ho bajtu
			I2C_Stop();
			return I2C_ERROR;
		}
	}

	if (!I2C_Read(&data[len - 1], false)) {  // NACK po posledn�m p�ijat�m bajtu
		I2C_Stop();
		return I2C_ERROR;
	}

	I2C_Stop();
	return I2C_OK;
}

uint8_t i2cWriteEeprom(uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len) {
	uint16_t i = 0;
	uint8_t firstReg, secondReg;

	if (len > 32) {
		return 0xFF;  // Omezen� d�lky na 32 bajt�
	}

	if (!I2C_Start()) {
		I2C_Stop();
		return I2C_ERROR;
	}

	if (!I2C_WAddr(addr, false)) {
		I2C_Stop();
		return I2C_ERROR;
	}

	firstReg = (reg >> 8) & 0xFF;
	secondReg = reg & 0xFF;

	if (!I2C_WData(firstReg) || !I2C_WData(secondReg)) {
		I2C_Stop();
		return I2C_ERROR;
	}

	for (i = 0; i < len; i++) {
		if (!I2C_WData(data[i])) {
			I2C_Stop();
			return I2C_ERROR;
		}
	}

	I2C_Stop();
	return I2C_OK;
}