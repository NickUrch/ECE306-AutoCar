//------------------------------------------------------------
//  Description: Contains Initial Configurations
//  Nick Urch
//  September 15, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------
#include "macros.h"
#include "functions.h"
#include "msp430.h"

extern char display_line[MAX_LCD_ROW][MAX_LCD_COL];
extern char *display[MAX_LCD_ROW];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern unsigned volatile char buff0Read[RNG_SIZE];
extern unsigned volatile char buff1Read[RNG_SIZE];
extern unsigned volatile char ring0Read;
extern unsigned volatile char ring0Write;
extern unsigned volatile char ring1Read;
extern unsigned volatile char ring1Write;
extern unsigned char proc0Read;
extern unsigned char proc1Read;
extern unsigned char proc0Write;
extern unsigned char proc1Write;
extern volatile unsigned char ADC_On;
extern unsigned char txPassThrough;
extern unsigned char inCommand;
extern unsigned int startUp_timer;
extern unsigned int startUp_counter;
extern unsigned char getIP;
extern unsigned char startIP;
extern unsigned char moreCommand;
extern unsigned char exCommand;
extern char check_point;

void Init_Conditions(void){
//------------------------------------------------------------------------------
	
  int i;
  for(i = LCD_COL0; i < MAX_LCD_COL; i++){
    display_line[LCD_ROW0][i] = RESET_STATE;
    display_line[LCD_ROW1][i] = RESET_STATE;
    display_line[LCD_ROW2][i] = RESET_STATE;
    display_line[LCD_ROW3][i] = RESET_STATE;
  }
  display_line[LCD_ROW0][LCD_COL10] = RESET_STATE;
  display_line[LCD_ROW1][LCD_COL10] = RESET_STATE;
  display_line[LCD_ROW2][LCD_COL10] = RESET_STATE;
  display_line[LCD_ROW3][LCD_COL10] = RESET_STATE;

  display[LCD_ROW0] = &display_line[LCD_ROW0][LCD_COL0];
  display[LCD_ROW1] = &display_line[LCD_ROW1][LCD_COL0];
  display[LCD_ROW2] = &display_line[LCD_ROW2][LCD_COL0];
  display[LCD_ROW3] = &display_line[LCD_ROW3][LCD_COL0];
  update_display = RESET_STATE;
	
	ring0Read = RESET;
	ring0Write = RESET;
	ring1Read = RESET;
	ring1Write = RESET;
	proc0Read = RESET;
	proc0Write = RESET;
	proc1Read = RESET;
	proc1Write = RESET;
	ADC_On = FALSE;
	txPassThrough = TRUE;
	inCommand = FALSE;
	startUp_timer = FALSE;
	getIP = FALSE;
	startIP = FALSE;
	moreCommand = FALSE;
	exCommand = TRUE;
	check_point = LCD_CHAR_ZERO;

// Interrupts are disabled by default, enable them.
  enable_interrupts();
//------------------------------------------------------------------------------
}

void Display_Process(void){
  if(update_display){
    update_display = RESET_STATE;
    if(display_changed){
      display_changed = RESET_STATE;
      Display_Update(RESET_STATE,RESET_STATE,RESET_STATE,RESET_STATE);
    }
  }
}

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns off both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}

void Init_Timers(void) {
	Init_Timer_B0();
	Init_Timer_B1();
	Init_Timer_B3();
}

//ADC Init
void Init_ADC(void){
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 2 A2
// V_DETECT_R (0x08) // Pin 3 A3
// V_THUMB (0x20) // Pin 5 A5
//------------------------------------------------------------------------------
// ADCCTL0 Register
ADCCTL0 = 0;													// Reset
ADCCTL0 |= ADCSHT_2;									// 16 ADC clocks
ADCCTL0 |= ADCMSC;										// MSC
ADCCTL0 |= ADCON;										// ADC ON
// ADCCTL1 Register
ADCCTL2 = 0;													// Reset
ADCCTL1 |= ADCSHS_0; 									// 00b = ADCSC bit
ADCCTL1 |= ADCSHP;										// ADC sample-and-hold SAMPCON signal from sampling timer.
ADCCTL1 &= ~ADCISSH; 									// ADC invert signal sample-and-hold.
ADCCTL1 |= ADCDIV_0; 									// ADC clock divider - 000b = Divide by 1
ADCCTL1 |= ADCSSEL_0;									// ADC clock MODCLK
ADCCTL1 |= ADCCONSEQ_0;							// ADC conversion sequence 00b = Single-channel single-conversion
// ADCCTL1 & ADCBUSY identifies a conversion is in process
// ADCCTL2 Register
ADCCTL2 = 0; // Reset
ADCCTL2 |= ADCPDIV0; 								// ADC pre-divider 00b = Pre-divide by 1
ADCCTL2 |= ADCRES_2; 								// ADC resolution 10b = 12 bit (14 clock cycle conversion time)
ADCCTL2 &= ~ADCDF; 									// ADC data read-back format 0b = Binary unsigned.
ADCCTL2 &= ~ADCSR; 									// ADC sampling rate 0b = ADC buffer supports up to 200 ksps
// ADCMCTL0 Register
ADCMCTL0 |= ADCSREF_0; 							// VREF - 000b = {VR+ = AVCC and VR– = AVSS }
ADCMCTL0 |= ADCINCH_2; 							// V_LEFT
ADCIE |= ADCIE0; 										// Enable ADC conv complete interrupt
ADCCTL0 &= ~ADCENC; 									// ADC enable conversion.
ADCCTL0 &= ~ADCSC; 										// ADC start conversion.
}

//Serial Comm UART 0 INIT
void Init_Serial_UCA0(void) {
	for (int i = 0; i < RNG_SIZE; i++) {
		buff0Read[i] = ZERO_BIT;
	}
	ring0Read = START;
	ring0Write = START;
	
	UCA0CTLW0 = 0;
	UCA0CTLW0 |= UCSWRST;
	UCA0CTLW0 |= UCSSEL__SMCLK;
	UCA0CTLW0 &= ~UCMSB;
	UCA0CTLW0 &= ~UCSPB;
	UCA0CTLW0 &= ~UCPEN;
	UCA0CTLW0 &= ~UCSYNC;
	UCA0CTLW0 &= ~UC7BIT;
	UCA0CTLW0 |= UCMODE_0;
	UCA0BRW = 4;
	UCA0MCTLW = 0x5551;
	UCA0CTLW0 &= ~UCSWRST;
	UCA0TXBUF = 0x00;
	UCA0IE |= UCRXIE;
}

//UART 1 INIT
void Init_Serial_UCA1(void) {
	for (int i = 0; i < RNG_SIZE; i++) {
		buff1Read[i] = ZERO_BIT;
	}
	ring1Read = START;
	ring1Write = START;
	
	UCA1CTLW0 = 0;
	UCA1CTLW0 |= UCSWRST;
	UCA1CTLW0 |= UCSSEL__SMCLK;
	UCA1CTLW0 &= ~UCMSB;
	UCA1CTLW0 &= ~UCSPB;
	UCA1CTLW0 &= ~UCPEN;
	UCA1CTLW0 &= ~UCSYNC;
	UCA1CTLW0 &= ~UC7BIT;
	UCA1CTLW0 |= UCMODE_0;
	UCA1BRW = 4;
	UCA1MCTLW = 0x5551;
	UCA1CTLW0 &= ~UCSWRST;
	UCA1TXBUF = 0x00;
	UCA1IE |= UCRXIE;
}

void Init_IoT(void) {
	startUp_counter = RESET;
	startUp_timer = TRUE;
}