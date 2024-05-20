// ------------------------------------------------------------------------------
//
//  Description: This file contains the Timer In
//
//  Nick Urch
//  Oct 4 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
// ------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include  "macros.h"
#include  <string.h>

extern char display_line[4][11];
extern volatile unsigned int Time_Sequence;
extern volatile unsigned char update_display;
extern unsigned int sw1_DB_time;
extern unsigned int sw2_DB_time;
extern unsigned int sw1_in_DB;
extern unsigned int sw2_in_DB;
extern unsigned int sw1_position;
extern unsigned int sw2_position;
extern volatile unsigned char ADC_On;
extern volatile unsigned int ADC_timer;
extern unsigned int display_menu;
extern unsigned int LCD_Menu;
extern unsigned int LCD_Prev;
extern unsigned int wait_timer;
extern unsigned int wait_counter;
extern unsigned int seek_timer;
extern unsigned int seek_counter;
extern unsigned int found_timer;
extern unsigned int found_counter;
extern unsigned int align_timer;
extern unsigned int align_counter;
extern unsigned int start_timer;
extern unsigned int total_time;
extern unsigned int setup_timer;
extern unsigned int setup_counter;
extern unsigned int follow_timer;
extern unsigned int follow_counter;
extern unsigned int exit_timer;
extern unsigned int exit_counter;
extern unsigned int runMode;
extern unsigned int foundLine;
extern char buff1Out[PROC_SIZE];
extern char buff0Out[PROC_SIZE];
extern unsigned char proc0Read;
extern unsigned char proc1Read;
extern unsigned int reset_IoT;
extern unsigned int startUp_timer;
extern unsigned int startUp_counter;
extern unsigned char getIP;
extern unsigned char startIP;
extern unsigned int wheel_timer;
extern unsigned int wheel_counter;
extern unsigned int wheelsON;
extern unsigned char exCommand;
extern unsigned char moreCommand;
extern char buffIPWrite;

//Timer B3 Initialization
void Init_Timer_B3(void) {
	TB3CTL = TBSSEL__SMCLK;
	TB3CTL |= MC__UP;
	TB3CTL |= TBCLR;
	
	TB3CCR0 = WHEEL_PERIOD;
	
	TB3CCTL1 = OUTMOD_7;
	R_FWD_SPEED = WHEEL_OFF;
	
	TB3CCTL2 = OUTMOD_7;
	R_REV_SPEED = WHEEL_OFF;
	
	TB3CCTL3 = OUTMOD_7;
	L_FWD_SPEED = WHEEL_OFF;
	
	TB3CCTL4 = OUTMOD_7;
	L_REV_SPEED = WHEEL_OFF;
}

//Timer B1 Initialization
void Init_Timer_B1(void) {
	TB1CTL |= TBSSEL__ACLK;						//Get from ACLK
	TB1CTL |= TBCLR;
	TB1CTL |= MC__CONTINUOUS;
	TB1CTL |= ID__1;
	TB1CTL &= ~TBIE;
	TB1CTL &= ~TBIFG;
	TB1EX0 = TBIDEX__8;
	TB1CCR0 = TB1CCR0_INTERVAL;
	TB1CCTL0 |= CCIE;
}

//Timer B0 Init
void Init_Timer_B0(void) {
	TB0CTL = TBSSEL__SMCLK;							// SMCLK source
	TB0CTL |= TBCLR;											// Resets TB0R, clock divider, count direction
	TB0CTL |= MC__CONTINOUS;							// Continuous up
	TB0CTL |= ID__2;											// Divide clock by 2
	TB0EX0 = TBIDEX__8;									// Divide clock by an additional 8
	TB0CCR0 = TB0CCR0_INTERVAL;					// CCR0
	TB0CCTL0 |= CCIE;										// CCR0 enable interrupt
	TB0CCR1 = TB0CCR1_INTERVAL;					// CCR1
	TB0CCTL1 |= CCIE;										// CCR1 enable interrupt
	//TB0CCR2 = TB0CCR2_INTERVAL;				// CCR2
	//TB0CCTL2 |= CCIE;									// CCR2 enable interrupt
	TB0CTL &= ~TBIE;											// Disable Overflow Interrupt
	TB0CTL &= ~TBIFG;										// Clear Overflow Interrupt flag
}

//Timer B0 Interrupt
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void) {
	// 100MS Timer
	
	if (start_timer) //Total Time on Course
		total_time++;
	//Wait timer for Wheels changing directions
	if (wait_timer && wait_counter++ > WAIT_INTERVAL) {
		wait_timer = FALSE;
	}
	//Setup timer for Black Line Follow
	if (setup_timer) {
		setup_counter++;
		runMode = SET_UP;
	}
	//Seek timer for Black Line
	if (seek_timer) {
		seek_counter++;
		runMode = SEEK;
	}
	//Found Black line timer
	if (found_timer && found_counter++ > FOUND_INTERVAL) {
		LCD_Menu = LCD_ADC;
		found_timer = FALSE;
		runMode = ALIGN;
		align_timer = TRUE;
		align_counter = RESET;
	}
	//Align car to black line
	if (align_timer && align_counter++ > ALIGN_INTERVAL) {
		LCD_Menu = LCD_ADC;
		runMode = IDLE;
		align_timer = FALSE;
	}
	//Timer for wheel movement
	if (wheel_timer && wheel_counter++ > wheelsON) {
		wheel_timer = FALSE;
		exCommand = FALSE;
		Wheels_Off();
		runMode = IDLE;
		if (moreCommand)
			Process_UART0_Command();
	}
	//Timer for Black Line follow
	if (follow_timer) {
		follow_counter++;
		runMode = FOLLOW;
	}
	//Timer to exit the black line circle
	if (exit_timer) {
		exit_counter++;
		runMode = EXIT_CIRCLE;
	}
	TB1CCR0 += TB1CCR0_INTERVAL;
}

//Timer B0
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) {
	//Timer B0 5ms
	//Display Update 200ms
	if (++Time_Sequence > DELAY_200MS) {
		Time_Sequence = RESET;
		display_menu = TRUE;
		update_display = TRUE;
	}
	//ADC Interrupt timer 50ms
	if (ADC_On && ++ADC_timer > DELAY_50MS) {
		ADC_timer = RESET;
		ADCCTL0 |= ADCENC; 									// ADC enable conversion.
		ADCCTL0 |= ADCSC; 										// ADC start conversion
	}
	TB0CCR0 += TB0CCR0_INTERVAL;
}

//Timer B1 Interrupt
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void) {
	//Timer B0 1-2 Overflow Interrupt Vector
	switch(__even_in_range(TB0IV, 14)) {
	case 0: break;
	case 2: 
		if (sw1_in_DB) {
			if (++sw1_DB_time > DB_TIME) {
				sw1_in_DB = FALSE;
				P4IFG &= ~SW1;
				P4IE |= SW1;
				sw1_position = FALSE;
				//Switch 1 Operation
			}
		}
		if (sw2_in_DB) {
			if (++sw2_DB_time > DB_TIME) {
				sw2_in_DB = FALSE;
				P2IFG &= ~SW2; // Clear all P2.6 interrupt flags
				P2IE |= SW2; // P4.2 interrupt enabled
				sw2_position = FALSE;
				//Switch 2 Operation
			}
		}
		TB0CCR1 += TB0CCR1_INTERVAL;
		break;
	case 4:
		//IOT Startup timer
		if (startUp_timer) {
			switch (++startUp_counter) {
			case IOT_1 :
				//IoT System store off
				strcpy(buff0Out, "AT+SYSSTORE=0\r\n");
				proc0Read = RESET;
				UCA0IE |= UCTXIE;
				break;
			case IOT_2 :
				//IoT Allow Connections
				strcpy(buff0Out, "AT+CIPMUX=1\r\n");
				proc0Read = RESET;
				UCA0IE |= UCTXIE;
				break;
			case IOT_3 :
				//IoT Set server port to 8862
				strcpy(buff0Out, "AT+CIPSERVER=1,8862\r\n");
				proc0Read = RESET;
				UCA0IE |= UCTXIE;
				break;
			case IOT_4 :
				//Get IP
				strcpy(buff0Out, "AT+CIFSR\r\n");
				proc0Read = RESET;
				LCD_Menu = LCD_CLR;
				UCA0IE |= UCTXIE;
				break;
			case IOT_5 :
				//Get IP Again.... for some reason it doesn't always capture
				strcpy(buff0Out, "AT+CIFSR\r\n");
				startUp_timer = FALSE;
				getIP = TRUE;
				startIP = FALSE;
				buffIPWrite = RESET;
				proc0Read = RESET;
				UCA0IE |= UCTXIE;
				break;
			default : break;
			}
		}
		TB0CCR2 += TB0CCR2_INTERVAL;
		break;
	case 14:
		break;
	default : break;
	}
}
	