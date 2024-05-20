//------------------------------------------------------------
//  Description: Contains LED files
//  Nick Urch
//  September 15, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------
#include "macros.h"
#include "functions.h"
#include "msp430.h"
#include <string.h>

extern unsigned int wheel_pattern;
extern volatile unsigned char display_changed;
extern char display_line[4][11];
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;
extern unsigned int show_ADC;
extern unsigned int LCD_Menu;
extern unsigned int LCD_Prev;
extern char IP1[TMP_IP_SIZE];
extern char IP2[TMP_IP_SIZE];
extern char msg[PROC_SIZE];
extern char check_point;

void Show_Display(void) {
	switch (LCD_Menu) {
	case LCD_MAIN :
		lcd_4line();
		strcpy(display_line[0], " PROJECT  ");
  	strcpy(display_line[1], "   TEN    ");
  	strcpy(display_line[2], "  ECE306  ");
  	strcpy(display_line[3], "nurch@ncsu");
		display_changed = TRUE;
		break;
	case LCD_WOLF :
		lcd_4line();
		strcpy(display_line[0], "   NCSU   ");
  	strcpy(display_line[1], " WOLFPACK ");
  	strcpy(display_line[2], "  ECE306  ");
  	strcpy(display_line[3], "nurch@ncsu");
		display_changed = TRUE;
		break;
	case LCD_ADC : {
		lcd_4line();
		char thumbVal[4];
		char leftVal[4];
		char rightVal[4];
		int mod = 10;
		int div = 1;
		for (int i = 0; i < 4; i++) {
			thumbVal[i] = ((ADC_Thumb % mod) / div) + ASCII_OFFSET;
			leftVal[i] = ((ADC_Left % mod) / div) + ASCII_OFFSET;
			rightVal[i] = ((ADC_Right % mod) / div) + ASCII_OFFSET;
			mod *= TEN_MULTIPLIER;
			div *= TEN_MULTIPLIER;
		}
		if (P2OUT & IR_LED) {
			strcpy(display_line[0], "  IR  ON  ");
		} else {
			strcpy(display_line[0], "  IR OFF  ");
		}
  	strcpy(display_line[1], "THUMB=");
		display_line[1][6] = thumbVal[3];
		display_line[1][7] = thumbVal[2];
		display_line[1][8] = thumbVal[1];
		display_line[1][9] = thumbVal[0];
	
  	strcpy(display_line[2], "L_DET=    ");
		display_line[2][6] = leftVal[3];
		display_line[2][7] = leftVal[2];
		display_line[2][8] = leftVal[1];
		display_line[2][9] = leftVal[0];
	
		strcpy(display_line[3], "R_DET=    ");
		display_line[3][6] = rightVal[3];
		display_line[3][7] = rightVal[2];
		display_line[3][8] = rightVal[1];
		display_line[3][9] = rightVal[0];
	
		display_changed = TRUE;
	} break;
	case LCD_IP :
		lcd_4line();
		strcpy(display_line[0], "   ncsu   ");
		strcpy(display_line[1], "IP Address");
		strcpy(display_line[2], IP1);
		strcpy(display_line[3], IP2);
		display_changed = TRUE;
		break;
	case LCD_SW1 :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], " SWITCH 1 ");
  	strcpy(display_line[2], "  ACTIVE  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_SW2 :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], " SWITCH 2 ");
  	strcpy(display_line[2], "  ACTIVE  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_CHKPT :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "  ARRIVED ");
  	strcpy(display_line[2], "          ");
		display_line[2][4] = '0';
		display_line[2][4] = check_point;
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_START :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   LINE   ");
  	strcpy(display_line[2], "   START  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_SEEK :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   SEEK   ");
  	strcpy(display_line[2], "   START  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_FOUND :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   LINE   ");
  	strcpy(display_line[2], "   FOUND  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_ALIGN :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   ALIGN  ");
  	strcpy(display_line[2], "   LINE   ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_FOLLOW :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "    BL    ");
		strcpy(display_line[2], "  TRAVEL  ");
		strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_CIRCLE :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "  TRAVEL  ");
  	strcpy(display_line[2], "  CIRCLE  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_EXIT :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   EXIT   ");
  	strcpy(display_line[2], "   CIRCLE ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_STOP :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "  COURSE  ");
  	strcpy(display_line[2], "  FINISH  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_ERROR :
		lcd_4line();
		strcpy(display_line[0], "**********");
  	strcpy(display_line[1], "   WHEEL  ");
  	strcpy(display_line[2], "   ERROR  ");
  	strcpy(display_line[3], "**********");
		display_changed = TRUE;
		break;
	case LCD_CLR :
		lcd_4line();
		strcpy(display_line[0], "          ");
  	strcpy(display_line[1], "          ");
  	strcpy(display_line[2], "          ");
  	strcpy(display_line[3], "          ");
		display_changed = TRUE;
		break;
	case LCD_CMD :
		lcd_BIG_mid();
		strcpy(display_line[0], " Command  ");
		display_line[1][0] = ' ';
		display_line[1][1] = ' ';
		display_line[1][2] = ' ';
		display_line[1][3] = msg[2];
		display_line[1][4] = msg[3];
		display_line[1][5] = msg[4];
		display_line[1][6] = ' ';
		display_line[1][7] = ' ';
		display_line[1][8] = ' ';
		display_line[1][9] = ' ';
		strcpy(display_line[2], "          ");
		display_changed = TRUE;
		break;
	default :
		break;
	}
}