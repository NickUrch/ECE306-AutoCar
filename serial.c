//------------------------------------------------------------------------------
//
//  Description: This file contains the MaSerial Files
//
//
//  Nick Urch
//  November 01 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//-----------------------------------------------------

//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK 		Baudrate 	UCOS16 	UCBRx 	UCFx 	UCSx neg pos neg pos
// 8000000 	4800 			1 			104 		2 		0xD6 -0.08 0.04 -0.10 0.14
// 8000000 	9600 			1 			52 			1 		0x49 -0.08 0.04 -0.10 0.14
// 8000000 	19200 		1 			26 			0 		0xB6 -0.08 0.16 -0.28 0.20
// 8000000 	57600 		1 			8 			10 		0xF7 -0.32 0.32 -1.00 0.36
// 8000000 	15200 		1 			4 			5 		0x55 -0.80 0.64 -1.12 1.76
// 8000000 	460800 		0 			1				7 		0 0x4A -2.72 2.56 -3.76 7.28
//
// IoT MAC Address : 90:38:0c:b0:4a:30
// Using Port : 8862
//
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>

extern char display_line[4][11];
extern unsigned int LCD_Menu;
extern unsigned volatile char buff0Read[RNG_SIZE];
extern unsigned volatile char buff1Read[RNG_SIZE];
extern char buff0Proc[PROC_SIZE];
extern char buff1Proc[PROC_SIZE];
extern char buff0Out[PROC_SIZE];
extern char buff1Out[PROC_SIZE];
extern unsigned char proc0Read;
extern unsigned char proc1Read;
extern unsigned char proc0Write;
extern unsigned char proc1Write;
extern unsigned int startUp_timer;
extern unsigned int startUp_counter;
extern unsigned int setup_timer;
extern unsigned int setup_counter;
extern unsigned int exit_counter;
extern unsigned int exit_timer;
extern unsigned int follow_timer;
extern char check_point;
extern char buffIP[BUFF_IP_SIZE];
extern char IP1[TMP_IP_SIZE];
extern char IP2[TMP_IP_SIZE];
extern unsigned int wheel_timer;
extern unsigned int wheel_counter;
extern unsigned int wheelsON;
extern char msg[PROC_SIZE];
extern unsigned int runMode;
extern unsigned char moreCommand;
extern unsigned char exCommand;
extern char buffMsg[PROC_SIZE];

//Process A command coming from UART 0 (IoT)
void Process_UART0_Command(void) {
	LCD_Menu = LCD_CLR;
	strcpy(msg, buff0Proc);
	if (msg[COM_PW1] == PW1_CHAR && msg[COM_PW2] == PW2_CHAR) {
		Wheels_Off();
		switch (msg[COM_NUM]) {
		case COM_FORWARD :
		case COM_REVERSE :
		case COM_LEFT :
		case COM_RIGHT :
		case COM_ZERO_PT :
			runMode = MANUAL;
			LCD_Menu = LCD_CMD;
			break;
		case COM_SEEK :
			//Blackline Seek/Follow
			runMode = SET_UP;
			setup_timer = TRUE;
			setup_counter = RESET;
			exCommand = FALSE;
			break;
		case COM_EXIT :
			//Exit Command
			follow_timer = FALSE;
			runMode = EXIT_CIRCLE;
			exit_timer = TRUE;
			exit_counter = RESET;
			exCommand = FALSE;
			break;
		case COM_CHECK_PT :
			if (msg[COM_TIME] == COM_PLUS)
				check_point++;
			else if (msg[COM_TIME] == COM_MINUS)
				check_point--;
			runMode = IDLE;
			LCD_Menu = LCD_CHKPT;
			exCommand = FALSE;
			break;
		case COM_HALT :
			 //Halt Command
			exCommand = FALSE;
			moreCommand = FALSE;
			Wheels_Off();
			wheel_timer = FALSE;
			runMode = IDLE;
			break;
		case COM_ADC :
			runMode = IDLE;
			ADCCTL0 |= ADCENC;
			ADCCTL0 |= ADCSC;
			P2OUT |= IR_LED;
			LCD_Menu = LCD_ADC;
			break;
		default : break;
		}
	}
}

//Process Command From UART 1
void Process_UART1_Command(void) {
	char cmd = buff1Proc[1];
	switch (cmd) {
	case COM_HELLO :
		strcpy(buff1Out, "Hello\n");
		break;
	case COM_INIT :
		strcpy(buff1Out, "Initializing\n");
		break;
	case COM_IOT_RESET :
			P3OUT &= ~IOT_EN_CPU;
			break;
	default :
		strcpy(buff1Out, "Invalid Command\n");
		break;
	}
	proc1Read = RESET;
	UCA1IE |= UCTXIE;
	strcpy(buff1Proc, "");
}

//Capture the IP Address and Split into 2 Strings for the LCD
void Parse_IP(void) {
	int dotCount = RESET;
	int j = RESET;
	strcpy(IP1, "");
	strcpy(IP2, "");
	for (int i = RESET; i < PROC_SIZE; i++) {
		if (dotCount < 2) {
			IP1[i] = buffIP[i];
		} else if (dotCount >= 2 && buffIP[i] != 0x00) {
			IP2[j++] = buffIP[i];
		} else if (buffIP[i] == 0x00) {
			break;
		}
		if (buffIP[i] == IP_DOT)
			dotCount++;
	}
	if (IP1[0] == 0x00) {
		startUp_timer = TRUE;
		startUp_counter = IOT_4;
	} else {
		char tmpIP1[TMP_IP_SIZE] = {0};
		char tmpIP2[TMP_IP_SIZE] ={0};
		int len;
		len = (TMP_IP_SIZE - strlen(IP1)) / 2;
		for (int i = 0; i < len; i++) {
			tmpIP1[i] = ' ';
		}
		strcat(tmpIP1, IP1);
		strcpy(IP1, tmpIP1);
		len = (TMP_IP_SIZE - strlen(IP2)) / 2;
		for (int i = 0; i < len; i++) {
			tmpIP2[i] = ' ';
		}
		strcat(tmpIP2, IP2);
		strcpy(IP2, tmpIP2);
		exCommand = FALSE;
		LCD_Menu = LCD_IP;
	}
}

