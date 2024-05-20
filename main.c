//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Nick Urch
//  August 24 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
volatile unsigned int Time_Sequence;
unsigned int Last_Time_Sequence; 			// a variable to identify Time_Sequence has changed
unsigned int time_change; 							// is an identifier that a change has occurre
unsigned int wait_timer;
unsigned int wait_counter;
unsigned int wheel_timer;
unsigned int wheel_counter;
unsigned int wheelsON;
unsigned int found_timer;
unsigned int found_counter;
unsigned int follow_timer;
unsigned int follow_counter;
unsigned int seek_timer;
unsigned int seek_counter;
unsigned int align_timer;
unsigned int align_counter;
unsigned int startUp_timer;
unsigned int startUp_counter;
unsigned int exit_timer;
unsigned int exit_counter;
unsigned int setup_timer;
unsigned int setup_counter;
unsigned int start_timer;
unsigned int total_time;
char check_point = '0';
unsigned int runMode = IDLE;
unsigned int port3_operation = USE_SMCLK;
volatile unsigned char ADC_On;
volatile unsigned int ADC_timer;
volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Thumb;
volatile unsigned int ADC_Left;
volatile unsigned int ADC_Right;
volatile unsigned int ADC_Channel;
unsigned int offRight;
unsigned int offLeft;
unsigned int foundLine;
unsigned char firstOff = NONE;
unsigned char firstOn = NONE;
unsigned int display_menu;
unsigned int LCD_Menu = LCD_MAIN;
unsigned int LCD_Prev = LCD_MAIN;
unsigned volatile char buff0Read[RNG_SIZE];
unsigned volatile char buff1Read[RNG_SIZE];
char buff0Proc[PROC_SIZE];
char buff1Proc[PROC_SIZE];
char buff0Out[PROC_SIZE];
char buff1Out[PROC_SIZE];
char msg[PROC_SIZE];
char buffMsg[PROC_SIZE];
unsigned char exCommand;
unsigned char moreCommand;
unsigned volatile char ring0Read;
unsigned volatile char ring0Write;
unsigned volatile char ring1Read;
unsigned volatile char ring1Write;
unsigned char proc0Read;
unsigned char proc1Read;
unsigned char proc0Write;
unsigned char proc1Write;
unsigned char txPassThrough;
unsigned char inCommand;
unsigned char getIP;
unsigned char startIP;
char buffIP[BUFF_IP_SIZE];
char buffIPWrite;
char IP1[TMP_IP_SIZE];
char IP2[TMP_IP_SIZE];

void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
	PM5CTL0 &= ~LOCKLPM5;
	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();   										// Initialize Clock System
	Init_Serial_UCA0();
	Init_Serial_UCA1();
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Time_Sequence = RESET_STATE;
	Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
	Init_LEDs();
	Init_ADC();
	enable_display_update();
	Init_IoT();
	
  display_changed = TRUE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    
  	Display_Process();                 // Update Display		
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
		
		Check_Wheels();
		
		switch (runMode) {
		case IDLE :
			Wheels_Off();
			break;
		case SET_UP :
			SetUp_Seek();
			break;
		case SEEK :
			Seek_Line();
			break;
		case ALIGN :
			Align_Line();
			break;
		case FOLLOW :
			Follow_Line();
			break;
		case EXIT_CIRCLE :
			Exit_Circle();
			break;
		case STOP_COURSE :
			Stop_Course();
			break;
		case MANUAL :
			Manual_Nav();
			break;
		default : 
			Wheels_Off();
			break;
		}
		
		if (display_menu) {
			Show_Display();
			display_menu = FALSE;
		}
		
		//UART 0 - Ring to Process Buffer
		int tmp0 = ring0Read;
		int tmp1 = ring1Read;
		char tmpChar;
		
		if (tmp0 != ring0Write) {
			tmpChar = buff0Read[ring0Read];
			buff0Read[ring0Read++] = 0x00;
			
			if (tmpChar == COM_CHAR) {
				inCommand = TRUE;
				proc0Write = START;
			} else if (inCommand && tmpChar == 0x0D) {
				buff0Proc[proc0Write] = tmpChar;
				proc0Write = START;
				inCommand = FALSE;
				Process_UART0_Command();
			} else if (inCommand) {
				buff0Proc[proc0Write++] = tmpChar;
			} else if (tmpChar == ITTALICS && getIP == TRUE) {
				if (startIP == TRUE) {
					startIP = FALSE;
					getIP = FALSE;
					Parse_IP();
				} else {
					buffIPWrite = RESET;
					startIP = TRUE;
				}
			} else if (startIP == TRUE) {
				buffIP[buffIPWrite++] = tmpChar;
			}
		}
		//UART 1 - Ring to Process Buffer
		if (tmp1 != ring1Write) {
			tmpChar = buff1Read[ring1Read];
			buff1Read[ring1Read++] = 0x00;
			if (ring1Read >= RNG_SIZE) 
				ring1Read = START;
			if (txPassThrough) {
				UCA0TXBUF = tmpChar;
			} else {
				if (tmpChar == 0x0D) {
					buff1Proc[proc1Write] = tmpChar;
					proc1Write = START;
					Process_UART1_Command();
					txPassThrough = TRUE;
				} else {
					buff1Proc[proc1Write++] = tmpChar;
				}
			}
		}
	}
}

