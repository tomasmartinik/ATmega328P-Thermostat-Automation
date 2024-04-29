/*
 * RTC.c
 *
 * Created: 27/03/2024 17:13:43
 */ 
 #include "rtc.h"

// Funkce pro prevod z desitkoveho do BCD formatu
uint8_t RTC_DecToBcd(uint8_t val){
	return ((val/10*16) + (val%10));
}

// Funkce pro prevod z BCD do desitkoveho formatu
uint8_t RTC_BcdToDec(uint8_t val){
	return ((val/16*10) + (val%16));
}

// Inicializace RTC modulu
void RTC_Enable(){
	I2C_Start();
	I2C_WAddr(RTC_I2C_ADR, 0);
	I2C_WData(0x00);
	I2C_WData(~(1<<7)&0xFF);
	I2C_Stop();
}

// Nastaveni casu v RTC modulu
void RTC_SetTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year){
	I2C_Start();
	I2C_WAddr(RTC_I2C_ADR, 0);
	I2C_WData(0x00);
	I2C_WData(~(1<<7)&RTC_DecToBcd(sec));
	I2C_WData(RTC_DecToBcd(min));
	I2C_WData(~(1<<6)&RTC_DecToBcd(hour));	
	I2C_WData(RTC_DecToBcd(day));
	I2C_WData(RTC_DecToBcd(date));
	I2C_WData(RTC_DecToBcd(month));
	I2C_WData(RTC_DecToBcd(year));
	I2C_Stop();
}

// Cteni aktualniho casu z RTC modulu
void RTC_ReadTime(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year){
	I2C_Start();
	I2C_WAddr(RTC_I2C_ADR, 0);
	I2C_WData(0x00);
	I2C_RepStart();
	I2C_WAddr(RTC_I2C_ADR, 1);
	uint8_t s = 0, m = 0, h = 0, d = 0, dt = 0, mo = 0, y = 0;
	I2C_Read(&s, 1);
	I2C_Read(&m, 1);
	I2C_Read(&h, 1);
	I2C_Read(&d, 1);
	I2C_Read(&dt, 1);
	I2C_Read(&mo, 1);
	I2C_Read(&y, 0);
	I2C_Stop();
	*sec = RTC_BcdToDec(s & 0x7F);
	*min = RTC_BcdToDec(m);
	*hour = RTC_BcdToDec(h & 0x3F);
	*day = RTC_BcdToDec(d);
	*date = RTC_BcdToDec(dt);
	*month = RTC_BcdToDec(mo);
	*year = RTC_BcdToDec(y);
}

// Nastaveni generatoru ctvereckove vlny na vystupu RTC modulu
void RTC_SetGen(uint8_t frequency){
	
	uint8_t tmpReg = 0;
	switch(frequency){
		case 1:		tmpReg = (1<<4); break;
		case 4:		tmpReg = (1<<4)|(1<<0); break;
		case 8:		tmpReg = (1<<4)|(1<<1); break;
		case 32:	tmpReg = (1<<4)|(1<<1)|(1<<0); break;
		default:	tmpReg = (1<<7);
	}
	I2C_Start();
	I2C_WAddr(RTC_I2C_ADR, 0);
	I2C_WData(0x07);
	I2C_WData(tmpReg);
	I2C_Stop();
	// Nastaveni pinu pro generovany obdelnikovy signal (SQW/OUT) pro externi preruseni
	DDRD &= ~(1 << SQ);
	EICRA |= (1 << ISC01); // Nastaveni ISC01 na 1 (nastupna hrana)
	EICRA &= ~(1 << ISC00); // Nastaveni ISC00 na 0 (sestupna hrana)
	EIMSK |= (1 << INT0); // Povoleni ext. preruseni INT0
	sei(); // Globální povolení p?erušení
}


/**Propojeni RTC modulu DS1307 s kitem**
		RTC		<-->	kit:
		SQ		<-->	PD2
		DS		<-->	--
		SCL		<-->	PC5
		SDA		<-->	PC4
		VCC		<-->	5V
		GND		<-->	GND
**************************************/