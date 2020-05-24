#include "utils.h"
#include "3140_accel.h"

/* output command to hold a key (by keycode) through UART */
void pressKey(char command[], int keycode) {
	sprintf(command, "press %d \n", keycode);
	uart_putString(command);
}

/* output command to release the hold on every given key (by keycode) through UART */
void releaseKeys(char command[], int keycodes[], int numKeys) {
	sprintf(command, "release "); // start building command
	
	int i = 8;
	int j = 0;
	while (i < 50 && j < numKeys) { // add sufficient number of '%d 's to command char array
		command[i] = 37; // '%'
		command[i+1] = 100; // 'd'
		command[i+2] = 32; // ' '
		sprintf(command, command, keycodes[j]);
		i+=3;
		j++;
	}
	j = i >= 50 ? 8 + (j+1)*2 : i; // calculate index two places after last '%d'
	command[j] = 10; // '\n'
	
	uart_putString(command);
}

int isOn = 0;

int main(void) {
	int clock = SystemCoreClock;
	
	LED_Initialize();
	LEDRed_On();
	
		/* Setup for on/off button */
	NVIC_EnableIRQ(PORTC_IRQn); // enable PORTC interrupts
	SIM->SCGC5 |= 1 << 11; // enable clock to PORT C
	PORTC->PCR[6] |= 001 << 8; // setup SW2 button as GPIO
	PORTC->PCR[6] |= 1001 << 16; // trigger interrupt on rising-edge
	PTC->PDDR &= ~(1 << 6); // setup pin as input
	
	I2C_ReleaseBus();
	
	I2C_Init();
	uart_init();
	
	uint8_t a = ACCEL_ReadWhoAmI();
	uint8_t b = ACCEL_getDefaultConfig();
	
	/* polling variables */
	char command[50]; // command format: "press <keycode> \n" or "release <keycode> + ... + <keycode> \n"
	SRAWDATA data;
	int isYAxis = 1;
	int axisValue = data.y;
	int jumpConstant = -1200;
	int duckConstant = 1200;
	int nullConstant = 300;
	int lastAction = -1;
	int currAction = -1;
	int keycodes[2] = {32, 40};
	int numKeys = sizeof(keycodes) / sizeof(keycodes[0]); // calculate number of keys
	
	/* indicate initial orientation to find which axis to watch for */
	ACCEL_getAccelDat(&data);
	if (data.z < nullConstant && data.z > nullConstant*-1 && data.y < -1970) isYAxis = 0;
	
	while(1) { // start loop for polling
		if(isOn) { // check if system should poll accelerometer
			ACCEL_getAccelDat(&data); // get accelerometer data
			axisValue = isYAxis ? data.y : data.z; // check different axis depending on initial orientation
			
			if (axisValue < nullConstant && axisValue > nullConstant*-1) {
				currAction = 0;
				if (currAction != lastAction) releaseKeys(command, keycodes, numKeys);
			} else if (axisValue < jumpConstant) {
				currAction = 1;
				if (currAction != lastAction) pressKey(command, keycodes[0]);
			} else if (axisValue > duckConstant) {
				currAction = 2;
				if (currAction != lastAction) pressKey(command, keycodes[1]);
			}
			
			lastAction = currAction;
		}
	}
	
	return 0;
}

/* SW2 button interrupt handler */
void PORTC_IRQHandler(void) {
	__disable_irq(); // disable interrupts just in case
	isOn = 1-isOn; // toggle on/off
	if (isOn) {
		LEDRed_Off();
		LEDGreen_On();
	} else {
		LEDGreen_Off();
		LEDRed_On();
	}
	PORTC->PCR[6] |= 1001 << 16; // reset interrupt
	__enable_irq(); // have to reenable before exiting
}