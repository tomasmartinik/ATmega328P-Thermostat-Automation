// state_machine.h
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "display.h"
#include "rtc.h"
#include "ADC.h"
#include "UART.h"
#include "encoder.h"
#include "timer.h"
#include "common_types.h"

extern TimeSettingType currentSetting; 

void stateMachineInit(void);
void processStateMachine(void);
void switchScreen(ScreenState screen);

#endif /* STATE_MACHINE_H_ */
