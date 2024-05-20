//------------------------------------------------------------
//  Description: CInterupts
//  Nick Urch
//  October 13, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include  "macros.h"

extern volatile unsigned int ADC_Channel;
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;
extern volatile unsigned int ADC_Channel;
extern unsigned volatile char buff0Read[RNG_SIZE];
extern unsigned volatile char buff1Read[RNG_SIZE];
extern unsigned volatile char ring0Read;
extern unsigned volatile char ring0Write;
extern unsigned volatile char ring1Read;
extern unsigned volatile char ring1Write;
extern char buff0Out[PROC_SIZE];
extern unsigned char proc0Write;
extern unsigned char proc0Read;
extern char buff1Out[PROC_SIZE];
extern unsigned char proc1Write;
extern unsigned char proc1Read;
extern unsigned char txPassThrough;
extern unsigned char inCommand;


//ADC Interupt
#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void) {
	switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
	case ADCIV_NONE:
		break;
	case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
												// before its previous conversion result was read.
		break;
	case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
		break;
	case ADCIV_ADCHIIFG: // Window comparator interrupt flags
		break;
	case ADCIV_ADCLOIFG: // Window comparator interrupt flag
		break;
	case ADCIV_ADCINIFG: // Window comparator interrupt flag
		break;
	case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
		ADCCTL0 &= ~ADCENC;
		switch (ADC_Channel++) {
		case (CH_LEFT) :
			ADCMCTL0 &= ~ADCINCH_2;
			ADCMCTL0 |= ADCINCH_3;
			ADC_Left = ADCMEM0;
			break;
		case (CH_RIGHT) :
			ADCMCTL0 &= ~ADCINCH_3;
			ADCMCTL0 |= ADCINCH_5;
			ADC_Right = ADCMEM0;
			break;
		case (CH_THUMB):
			ADCMCTL0 &= ~ADCINCH_5;
			ADCMCTL0 |= ADCINCH_2;
			ADC_Thumb = ADCMEM0;
			ADC_Channel = RESET;
			break;
		default : break;
		}
		ADCCTL0 |= ADCENC;
		ADCCTL0 |= ADCSC;
		break;
	default:
		break;
	}
}

//Serial Interrupt 0
#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void) {
	char tmpChar;
	unsigned char tmp;
	switch(__even_in_range(UCA0IV, 0x08)) {
	case 0 :
		break;
	case 2 :
		tmpChar = UCA0RXBUF;
		UCA1TXBUF = tmpChar;
		tmp = ring0Write++;
		buff0Read[tmp] = tmpChar;
		if (ring0Write >= (sizeof(buff0Read))) {
			ring0Write = START;
		}
		break;
	case 4 :
		UCA0TXBUF = buff0Out[proc0Read++];
		if (buff0Out[proc0Read] == 0x00)
			UCA0IE &= ~UCTXIE;
		break;
	default :
		break;
	}
}

//UART 1 Interrupt
#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
	unsigned char tmp;
	switch(__even_in_range(UCA1IV, 0x08)) {
	case 0:
		break;
	case 2:
		tmp = ring1Write++;
		buff1Read[tmp] = UCA1RXBUF;
		if (buff1Read[tmp] == COM_CHAR)
			txPassThrough = FALSE;
		if (ring1Write >= (sizeof(buff1Read))) {
			ring1Write = START;
		}
		break;
	case 4:
		//Transmit
		UCA1TXBUF = buff1Out[proc1Read++];
		if (buff1Out[proc1Read] == 0x00)
			UCA1IE &= ~UCTXIE;
		break;
	default:
		break;
	}
}