//------------------------------------------------------------------------------
//
//  Description: This file contains the MaRun Mode State Machines
//	Nick Urch
//  October 14 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"

extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;
extern unsigned int offRight;
extern unsigned int offLeft;
extern unsigned int foundLine;
extern volatile signed int delta;
extern unsigned int wait_timer;
extern unsigned int wait_counter;
extern unsigned int seek_timer;
extern unsigned int seek_counter;
extern unsigned int found_timer;
extern unsigned int found_counter;
extern unsigned int setup_timer;
extern unsigned int setup_counter;
extern unsigned int align_timer;
extern unsigned int align_counter;
extern unsigned int follow_timer;
extern unsigned int follow_counter;
extern unsigned int exit_counter;
extern unsigned int exit_timer;
extern unsigned int start_timer;
extern unsigned int total_time;
extern unsigned int LCD_Menu;
extern unsigned int runMode;
extern unsigned char firstOff;
extern unsigned char firstOn;
extern unsigned int wheel_timer;
extern unsigned int wheel_counter;
extern unsigned int wheelsON;
unsigned char onWhite;
extern char msg[PROC_SIZE];

//Set up the vehicle for Black Line Follow
void SetUp_Seek(void){
	LCD_Menu = LCD_START;
	//Forward
	if (setup_counter > WAIT_TIME_0 && setup_counter <= WAIT_TIME_25)
		Run_Forward(WHEEL_SLOW);
	//Turn Left
	else if (setup_counter > WAIT_TIME_25 && setup_counter <= WAIT_TIME_40)
		Forward_Turn(WHEEL_CRAWL, WHEEL_SLOW);
	else if (setup_counter > WAIT_TIME_40) {
		Wheels_Off();
		setup_timer = FALSE;
		ADCCTL0 |= ADCENC;
		ADCCTL0 |= ADCSC;
		P2OUT |= IR_LED;
		seek_timer = TRUE;
		seek_counter = RESET;
		onWhite = FALSE;
	}
}

void Seek_Line(void) {
	if (seek_counter > WAIT_TIME_0 && seek_counter <= WAIT_TIME_50) {
		LCD_Menu = LCD_SEEK;
	} else if (seek_counter > WAIT_TIME_50 && seek_counter <= WAIT_TIME_150) {
		LCD_Menu = LCD_ADC;
		Check_ADC();
		Run_Forward(WHEEL_VSLOW);
		//If on the white board, allow for black line find.
		if (onWhite) {
			if (!foundLine && !wait_timer) {
				if (offLeft && offRight) {
					Run_Forward(WHEEL_VSLOW);
				} else if (!offLeft || !offRight) {
					if (!offLeft) {
						firstOn = LEFT;
					} else {
						firstOn = RIGHT;
					}
					foundLine = TRUE;
				}
			} else if (foundLine) {
				//Found Black Line, allow for Align
				Wheels_Off();
				runMode = IDLE;
				LCD_Menu = LCD_FOUND;
				seek_timer = FALSE;
				found_timer = TRUE;
				found_counter = RESET;
			}
		}
	} else if (seek_counter > WAIT_TIME_150) {
		runMode = IDLE;
		Wheels_Off();
		LCD_Menu = LCD_IP;
		seek_timer = FALSE;
	}
}

//Align the vehicle to the Black Line
void Align_Line(void) {
	LCD_Menu = LCD_ALIGN;
	Check_ADC();

	Zero_Point_Right(WHEEL_VSLOW);

	if (!offLeft && !offRight) {
		runMode = IDLE;
		Wheels_Off();
		align_timer = FALSE;
		follow_counter = RESET;
		follow_timer = TRUE;
	}
}

//Follow the Black Line. Bang Bang Method.
void Follow_Line(void) {
	if (follow_counter <= WAIT_TIME_50) {
		LCD_Menu = LCD_ALIGN;
	} else if (follow_counter > WAIT_TIME_50) {
		if (follow_counter < WAIT_TIME_100)
			LCD_Menu = LCD_FOLLOW;
		else
			LCD_Menu = LCD_CIRCLE;
		
		//Check ADC if off Left, turn right/ if off Right, turn Left
		Check_ADC();
		if (!offLeft && !offRight)
			Run_Forward(WHEEL_VSLOW);
		else if (offLeft)
			Forward_Turn(WHEEL_VSLOW, WHEEL_OFF);
		else if (offRight)
			Forward_Turn(WHEEL_OFF, WHEEL_VSLOW);
	}
}

//Exit the Circle
void Exit_Circle(void) {
	LCD_Menu = LCD_EXIT;
	if (exit_counter <= WAIT_TIME_50) {
		Wheels_Off();
		LCD_Menu = LCD_EXIT;
	} else if (exit_counter > WAIT_TIME_50 && exit_counter <= WAIT_TIME_60) {
		Forward_Turn(WHEEL_OFF, WHEEL_SLOW);
	} else if (exit_counter > WAIT_TIME_60 && exit_counter <= WAIT_TIME_75) {
		Run_Forward(WHEEL_SLOW);
	} else if (exit_counter > WAIT_TIME_75) {
		exit_timer = FALSE;
		Wheels_Off();
		runMode = STOP_COURSE;
	}
}

//Course Finished! Update Display to TA
void Stop_Course() {
	LCD_Menu = LCD_STOP;
	runMode = IDLE;
	ADCCTL0 &= ~ADCENC;
	ADCCTL0 &= ~ADCSC;
	P2OUT &= ~IR_LED;
}

//Manual Navigation Mode
void Manual_Nav(void) {
	LCD_Menu = LCD_CMD;
	if (!start_timer) {
		start_timer = TRUE;
		total_time = RESET;
	}
	char cmd = msg[COM_NUM];
	char speed = msg[COM_SPEED];
	char time = msg[COM_TIME];
	if (!wheel_timer) {
		wheel_timer = TRUE;
		wheel_counter = RESET;
		wheelsON = (time - ASCII_OFFSET) * TIMER_MULTIPLIER;
	}
	switch (cmd) {
	case COM_FORWARD :
		if (speed == COM_FAST)
			Run_Forward(WHEEL_FAST);
		else if (speed == COM_HALF)
			Run_Forward(WHEEL_HALF);
		else if (speed == COM_SLOW)
			Run_Forward(WHEEL_SLOW);
		break;
	case COM_REVERSE :
		if (speed == COM_FAST)
			Run_Reverse(WHEEL_FAST);
		else if (speed == COM_HALF)
			Run_Reverse(WHEEL_HALF);
		else if (speed == COM_SLOW)
			Run_Reverse(WHEEL_SLOW);
		break;
	case COM_LEFT :
		if (speed == COM_FAST)
			Forward_Turn(WHEEL_HALF, WHEEL_FAST);
		else if (speed == COM_HALF)
			Forward_Turn(WHEEL_SLOW, WHEEL_HALF);
		else if (speed == COM_SLOW)
			Forward_Turn(WHEEL_OFF, WHEEL_SLOW);
		break;
	case COM_RIGHT :
		if (speed == COM_FAST)
			Forward_Turn(WHEEL_FAST, WHEEL_HALF);
		else if (speed == COM_HALF)
			Forward_Turn(WHEEL_HALF, WHEEL_SLOW);
		else if (speed == COM_SLOW)
			Forward_Turn(WHEEL_SLOW, WHEEL_OFF);
		break;
	case COM_ZERO_PT :
		if (speed == COM_LEFT)
			Zero_Point_Left(WHEEL_SLOW);
		else if (speed == COM_RIGHT)
			Zero_Point_Right(WHEEL_SLOW);
		break;
	default : break;
	}
}

void Check_ADC(void) {
	if (ADC_Left < L_OFF) {
		offLeft = TRUE;
		//Left is off
	} else {
		//Found line on Left Edge
		offLeft = FALSE;
	}
		
	if (ADC_Right < R_OFF) {
		offRight = TRUE;
		//Right is off
	} else {
		//Found line on Right Edge
		offRight = FALSE;
	} 
	
	if (!onWhite) {
		if (ADC_Left < ON_WHITE && ADC_Right < ON_WHITE)
			onWhite = TRUE;
	}
	
	if (!offRight && !offLeft)
		firstOff = NONE;
	else if (firstOff == NONE && offRight)
		firstOff = RIGHT;
	else if (firstOff == NONE && offLeft)
		firstOff = LEFT;
}