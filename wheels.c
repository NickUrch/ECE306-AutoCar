//------------------------------------------------------------
//  Description: Contains Wheel Control files
//  Nick Urch
//  September 15, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------

#include "macros.h"
#include "functions.h"
#include "msp430.h"

extern unsigned int event;
extern unsigned int wait_counter;
extern unsigned int wait_timer;
extern unsigned int runMode;
extern unsigned int LCD_Menu;

//Forward
void Run_Forward(unsigned int speed) {
	if (!wait_timer) {
		R_FWD_SPEED = speed + RIGHT_OFFSET;
		L_FWD_SPEED = speed + LEFT_OFFSET;
	}
}

//Reverse
void Run_Reverse(unsigned int speed) {
	if (!wait_timer) {	
		R_REV_SPEED = speed + RIGHT_OFFSET;
		L_REV_SPEED = speed + LEFT_OFFSET;
	}
}

//Zero Point Turn Right
void Zero_Point_Right(unsigned int speed) {
	if (!wait_timer) {
		R_REV_SPEED = speed + RIGHT_OFFSET;
		L_FWD_SPEED = speed + LEFT_OFFSET;
	}
}

//Zero Point Turn Left
void Zero_Point_Left(unsigned int speed) {
	if (!wait_timer) {
		R_FWD_SPEED = speed + RIGHT_OFFSET;
		L_REV_SPEED = speed + LEFT_OFFSET;
	}
}

//Forward Turn
void Forward_Turn(unsigned int lSpeed, unsigned int rSpeed) {
	if (!wait_timer) {
		R_FWD_SPEED = rSpeed + RIGHT_OFFSET;
		L_FWD_SPEED = lSpeed + LEFT_OFFSET;
	}
}

//Reverse Turn
void Reverse_Turn(unsigned int lSpeed, unsigned int rSpeed) {
	if (!wait_timer) {
		R_REV_SPEED = rSpeed + RIGHT_OFFSET;
		L_REV_SPEED = lSpeed + LEFT_OFFSET;
	}
}

//Turn all wheels off
void Wheels_Off(void) {
	R_FWD_SPEED = WHEELS_OFF;
	R_REV_SPEED = WHEELS_OFF;
	L_FWD_SPEED = WHEELS_OFF;
	L_REV_SPEED = WHEELS_OFF;
	wait_counter = RESET;
	wait_timer = TRUE;
}


//Make sure Forward and Reverse are not on at the same time (Magic Smoke)
void Check_Wheels(void) {
	if ((R_FWD_SPEED && R_REV_SPEED) || (L_FWD_SPEED && L_REV_SPEED)) {
		Wheels_Off();
		runMode = IDLE;
		wait_timer = TRUE;
		wait_counter = RESET;
		LCD_Menu = LCD_ERROR;
	}
}