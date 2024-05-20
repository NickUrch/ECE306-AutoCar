//------------------------------------------------------------------------------
//
//  Description: This File controls the switch process
//
//
//  Nick Urch
//  September 25, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"

unsigned int sw1_DB_time = RESET;
unsigned int sw2_DB_time = RESET;
unsigned int sw1_in_DB = RESET;
unsigned int sw2_in_DB = RESET;
unsigned int sw1_position = RESET;
unsigned int sw2_position = RESET;
extern unsigned int event;
extern unsigned int wheel_timer;
extern unsigned int wheel_state;
extern unsigned int wait_timer;
extern unsigned int wheel_pattern;
extern unsigned int port3_operation;
extern unsigned int LCD_Menu;
extern unsigned int LCD_Prev;

#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
	//Switch 1
	if (P4IFG & SW1) {
		P4IFG &= ~SW1;
		sw1_position = TRUE;
		sw1_DB_time = RESET;
		sw1_in_DB = TRUE;
		P4IFG &= ~SW1; // Clear all P4 interrupt flags
		P4IE &= ~SW1; // P4.2 interrupt disabled
		LCD_Prev = LCD_Menu;
		LCD_Menu = LCD_SW1;
	}
}

#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
	//Switch 2
	if (P2IFG & SW2) {
		P2IFG &= ~SW2;
		//P2OUT |= IR_LED;
		sw2_position = TRUE;
		sw2_DB_time = RESET;
		sw2_in_DB = TRUE;
		P2IFG &= ~SW2; // Clear all P2.6 interrupt flags
		P2IE &= ~SW2; // P4.2 interrupt disabled
		LCD_Prev = LCD_Menu;
		LCD_Menu = LCD_SW2;
	}
}
