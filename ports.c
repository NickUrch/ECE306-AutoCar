//------------------------------------------------------------
//  Description: CInitial Port Values
//  Nick Urch
//  September 15, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------

#include "macros.h"
#include "functions.h"
#include "msp430.h"

extern unsigned int port3_operation;    //Set 3.4 to SMCLK or GPIO

void Init_Ports (void) {
	Init_Port1();
	Init_Port2();
	switch (port3_operation) {
	case USE_GPIO : Init_Port3(USE_GPIO); break;
	case USE_SMCLK : Init_Port3(USE_SMCLK); break;
	default : Init_Port3(USE_GPIO); break;
	}
	Init_Port4();
	Init_Port5();
	Init_Port6();
}

void Init_Port1(void) {
	P1DIR = ZERO_BIT;
	P1OUT = ZERO_BIT;
	
	P1SEL0 &= ~RED_LED;
	P1SEL1 &= ~RED_LED;
	P1OUT |= RED_LED;
	P1DIR |= RED_LED;
	
	P1SELC |= A1_SEEED;
	P1SELC |= V_DETECT_L;
	P1SELC |= V_DETECT_R;
	P1SELC |= A4_SEEED;
	P1SELC |= V_THUMB;
	
	P1SEL0 |= UCA0RXD;
	P1SEL1 &= ~UCA0RXD;
	
	P1SEL0 |= UCA0TXD;
	P1SEL1 &= ~UCA0TXD;
}

void Init_Port2(void) {
	P2OUT = 0x00; // P2 set Low
	P2DIR = 0x00; // Set P2 direction to output
	
	P2SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
	P2SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
	P2OUT &= ~RESET_LCD; // Initial Value = Low / Off
	P2DIR |= RESET_LCD; // Direction = output
	
	P2SEL0 &= ~Check_Bat; // Check_Bat GPIO operation
	P2SEL1 &= ~Check_Bat; // Check_Bat GPIO operation
	P2OUT &= ~Check_Bat; // Initial Value = Low / Off
	P2DIR |= Check_Bat; // Direction = output
	
	P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
	P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
	P2OUT &= ~IR_LED; // Initial Value = Low / Off
	P2DIR |= IR_LED; // Direction = input
	
	P2SEL0 &= ~SW2; // SW2 Operation
	P2SEL1 &= ~SW2; // SW2 Operation
	P2OUT |= SW2; // Configure pullup resistor
	P2DIR &= ~SW2; // Direction = input
	P2REN |= SW2; // Enable pullup resistor
	P2IES |= SW2; // Hi/Lo edge interrupt
	P2IFG &= ~SW2; // Clear all P .26 interrupt flags
	P2IE |= SW2; // interrupt enabled
	
	P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
	P2SEL1 &= ~IOT_RUN_CPU; // IOT_RUN_CPU GPIO operation
	P2OUT &= ~IOT_RUN_CPU; // Initial Value = Low / Off
	P2DIR |= IOT_RUN_CPU; // Direction = input
	
	P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
	P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
	P2OUT |= DAC_ENB; // Initial Value = High
	P2DIR |= DAC_ENB; // Direction = output
	
	P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
	P2SEL1 |= LFXOUT; // LFXOUT Clock operation
	
	P2SEL0 &= ~LFXIN; // LFXIN Clock operation
	P2SEL1 |= LFXIN; // LFXIN Clock operation
}

void Init_Port3(char smclk) {
	P3SEL0 &= ~TEST_PROBE;
	P3SEL1 &= ~TEST_PROBE;
	P3OUT &= ~TEST_PROBE;
	P3DIR |= TEST_PROBE;
	
	P3SEL0 &= ~OA2O;
	P3SEL1 &= ~OA2O;
	P3OUT &= ~OA2O;
	P3DIR |= OA2O;
	
	P3SEL0 &= ~OA2N;
	P3SEL1 &= ~OA2N;
	P3OUT &= ~OA2N;
	P3DIR |= OA2N;
	
	P3SEL0 &= ~OA2P;
	P3SEL1 &= ~OA2P;
	P3OUT &= ~OA2P;
	P3DIR |= OA2P;
	
	switch (smclk) {
	case (USE_SMCLK) : 
		P3SEL0 |= SMCLK;
		P3SEL1 &= ~SMCLK;
		//P3OUT &= ~SMCLK;
		P3DIR |= SMCLK;
		break;
	case (USE_GPIO) : 
		P3SEL0 &= ~SMCLK;
		P3SEL1 &= ~SMCLK;
		P3OUT &= ~SMCLK;
		P3DIR |= SMCLK;
		break;
	}
		
	P3SEL0 &= ~DAC_CNTL;
	P3SEL1 &= ~DAC_CNTL;
	P3OUT &= ~DAC_CNTL;
	P3DIR |= DAC_CNTL;
	
	P3SEL0 &= ~IOT_LINK_CPU;
	P3SEL1 &= ~IOT_LINK_CPU;
	P3OUT &= ~IOT_LINK_CPU;
	P3DIR |= IOT_LINK_CPU;
	
	P3SEL0 &= ~IOT_EN_CPU;
	P3SEL1 &= ~IOT_EN_CPU;
	P3OUT &= ~IOT_EN_CPU;
	P3DIR |= IOT_EN_CPU;
}

void Init_Port4(void) {
	P4OUT = ZERO_BIT; // P4 set Low
	P4DIR = ZERO_BIT; // Set P4 direction to output
	
	P4SEL0 &= ~DAC_CNTL1; // DAC_CNTL1 GPIO operation
	P4SEL1 &= ~DAC_CNTL1; // DAC_CNTL1 GPIO operation
	P4DIR &= ~DAC_CNTL1; // Set DAC_CNTL1 direction to input
	
	P4SEL0 &= ~SW1; // SW1 GPIO operation
	P4SEL1 &= ~SW1; // SW1 GPIO operation
	P4OUT |= SW1; // Configure pullup resistor
	P4DIR &= ~SW1; // Direction = input
	P4REN |= SW1; // Enable pullup resistor
	P4IES |= SW1; // P4.2 Hi/Lo edge interrupt
	P4IFG &= ~SW1; // Clear all P2.6 interrupt flags
	P4IE |= SW1; // P4.2 interrupt enabled
	
	P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
	P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation
	
	P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
	P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation
	
	P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
	P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
	P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
	P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output
	
	P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
	P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation
	
	P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
	P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation
	
	P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
	P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
}

void Init_Port5(void) {
	P5DIR = ZERO_BIT;
	P5OUT = ZERO_BIT;
	
	P5SELC |= CHECK_BAT;
	P5SELC |= V_BAT;
	P5SELC |= V_DAC;
	P5SELC |= V_3_3;
	
	P5SEL0 &= ~IOT_BOOT_CPU;
	P5SEL1 &= ~IOT_BOOT_CPU;
	P5OUT |= IOT_BOOT_CPU;
	P5DIR |= IOT_BOOT_CPU;
}

void Init_Port6(void) {
	
	//R Forward
	P6SEL0 |= R_FORWARD;
	P6SEL1 &= ~R_FORWARD;
	P6DIR |= R_FORWARD;
	
	//R Reverse
	P6SEL0 |= R_REVERSE;
	P6SEL1 &= ~R_REVERSE;
	P6DIR |= R_REVERSE;
	
	//L Forward
	P6SEL0 |= L_FORWARD;
	P6SEL1 &= ~L_FORWARD;
	P6DIR |= L_FORWARD;
	
	//L Reverse
	P6SEL0 |= L_REVERSE;
	P6SEL1 &= ~L_REVERSE;
	P6DIR |= L_REVERSE;
	
	//LCD Backlite
	P6SEL0 &= ~LCD_BACKLITE;
	P6SEL1 &= ~LCD_BACKLITE;
	P6OUT |= LCD_BACKLITE;
	P6DIR |= LCD_BACKLITE;
	
	//Green LED
	P6SEL0 &= ~GRN_LED;
	P6SEL1 &= ~GRN_LED;
	P6OUT |= GRN_LED;
	P6DIR |= GRN_LED;
	
}