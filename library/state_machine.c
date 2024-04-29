#include "state_machine.h"
#include "menu.h"

ScreenState currentState = MAIN_SCREEN;
static int lastButtonCount = 0;
static int lastEncoderValue;
TimeSettingType currentSetting = SET_DAY;  

void stateMachineInit(void) {
	encoderInit();  
	lastEncoderValue = encoderRotation(); 
}

void processStateMachine(void) {
	int currentButtonCount = encoderBtn();  
	int currentEncoderValue = encoderRotation(); 
	int rotation = currentEncoderValue - lastEncoderValue; 

	// Reaguje na stisknutí tlačítka
	if (currentButtonCount != lastButtonCount) {
		lastButtonCount = currentButtonCount;  
		if (currentState == SETTINGS_DATE) {
			switchTimeSetting();  
			} else {
			freezeCurrentTime();
			currentState = (ScreenState)((currentState + 1) % NUM_SCREENS);  // Cyklické přepínání mezi obrazovkami
		}
	}

	// Reaguje na rotaci enkodéru
	if (rotation != 0) {
		if (currentState == SETTINGS_DATE) {
			updateSettingsTime(rotation);  
			} else {
			updateSetting(currentState, rotation);  
		}
		lastEncoderValue = currentEncoderValue;  
	}
	switchScreen(currentState);
}

void switchScreen(ScreenState screen) {
	switch (screen) {
		case MAIN_SCREEN:
		displayMainScreen();
		break;
		case SETTINGS_DATE:
		displaySettingsSecond();
		break;
		case SETTINGS_HYSTERESIS:
		displaySettingsFirst();
		break;
		case SETTINGS_CALIBRATION:
		displaySettingsThird();
		break;
	}
}
