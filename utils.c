#include <MK64F12.h>
#include "utils.h"

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_Initialize(void) {
  SIM->SCGC5 |= (1 << 10) | (1 << 13);  /* Enable Clock to Port B & E */ 
  PORTB->PCR[22] = (1 << 8);               /* Pin PTB22 is GPIO */
  PORTB->PCR[21] = (1 << 8);                /* Pin PTB21 is GPIO */
  PORTE->PCR[26] = (1 << 8);                /* Pin PTE26  is GPIO */
  
  PTB->PDOR = (1 << 21 | 1 << 22);          /* switch Red/Green LED off  */
  PTB->PDDR = (1 << 21 | 1 << 22);          /* enable PTB18/19 as Output */

  PTE->PDOR = 1 << 26;            /* switch Blue LED off  */
  PTE->PDDR = 1 << 26;            /* enable PTE26 as Output */
}

/*----------------------------------------------------------------------------
  Function that toggles the Red LED
 *----------------------------------------------------------------------------*/

void LEDRed_Toggle (void) {
	PTB->PTOR |= 1 << 22; 	   /* Red LED Toggle */
}

/*----------------------------------------------------------------------------
  Function that toggles the Blue LED
 *----------------------------------------------------------------------------*/
void LEDBlue_Toggle (void) {
	PTB->PTOR |= 1 << 21; 	   /* Blue LED Toggle */
}

/*----------------------------------------------------------------------------
  Function that toggles the Green LED
 *----------------------------------------------------------------------------*/
void LEDGreen_Toggle (void) {
	PTE->PTOR |= 1 << 26; 	   /* Green LED Toggle */
}

/*----------------------------------------------------------------------------
  Function that turns on Red LED
 *----------------------------------------------------------------------------*/
void LEDRed_On (void) {
  PTB->PCOR |= 1 << 22;   /* Red LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Green LED
 *----------------------------------------------------------------------------*/
void LEDGreen_On (void) {
  PTE->PCOR |= 1 << 26;   /* Green LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Blue LED
 *----------------------------------------------------------------------------*/
void LEDBlue_On (void) {
  PTB->PCOR |= 1 << 21;   /* Blue LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns off Red LED
 *----------------------------------------------------------------------------*/
void LEDRed_Off (void) {
  PTB->PSOR |= 1 << 22;   /* Red LED Off*/
}

/*----------------------------------------------------------------------------
  Function that turns off Green LED
 *----------------------------------------------------------------------------*/
void LEDGreen_Off (void) {	
  PTE->PSOR |= 1 << 26;   /* Green LED Off*/
}

/*----------------------------------------------------------------------------
  Function that turns off Blue LED
 *----------------------------------------------------------------------------*/
void LEDBlue_Off (void) {	
  PTB->PSOR |= 1 << 21;   /* Blue LED Off*/
}

/*----------------------------------------------------------------------------
  Function that turns all LEDs off
 *----------------------------------------------------------------------------*/
void LED_Off (void) {	
	// Save and disable interrupts (for atomic LED change)
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq();
	
  PTE->PSOR |= 1 << 26;   /* Green LED Off*/
  PTB->PSOR |= 1 << 22;   /* Red LED Off*/
  PTB->PSOR |= 1 << 21;   /* Blue LED Off*/
	
	// Restore interrupts
	__set_PRIMASK(m);
}

void LEDRed_Blink() {
	LEDRed_On();
	delay();
	LEDRed_Off();
}

void LEDGreen_Blink() {
	LEDGreen_On();
	delay();
	LEDGreen_Off();
}

void LEDBlue_Blink() {
	LEDBlue_On();
	delay();
	LEDBlue_Off();
}

void delay(void){
	int j;
	for(j=0; j<1000000; j++);
}

void multiDelay(int times) {
	for (int i = 0; i < times; i++) delay();
}