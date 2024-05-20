//------------------------------------------------------------
//  Description: Contains Defined Values for Other Files
//  Nick Urch
//  September 15, 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------

#define ALWAYS                   (1)
#define ZERO_BIT									(0)
#define RESET_STATE              (0)
#define RESET											(0)
#define BEGIN											(1)
#define TRUE                 (0x01) //
#define FALSE									 		(0)
#define TEST_PROBE           (0x01) // 3.0 TEST PROBE

//Switches
#define DB_TIME									(15)

//LCD DEFINES
#define MAX_LCD_COL					(0x0B) // Max Digits (columns) of LCD
#define MAX_LCD_ROW					(0x04) // Max rows of LCD
#define MAX_CHARS					(10)
#define LCD_ROW0						(0x00) // Row 0
#define LCD_ROW1						(0x01) // Row 1
#define LCD_ROW2						(0x02) // Row 2
#define LCD_ROW3						(0x03) // Row 3
#define LCD_COL0						(0x00) // Column 0
#define LCD_COL1						(0x00) // Column 1
#define LCD_COL2						(0x00) // Column 2
#define LCD_COL3						(0x00) // Column 3
#define LCD_COL4						(0x00) // Column 4
#define LCD_COL5						(0x00) // Column 5
#define LCD_COL6						(0x00) // Column 6
#define LCD_COL7						(0x00) // Column 7
#define LCD_COL8						(0x00) // Column 8
#define LCD_COL9						(0x00) // Column 9
#define LCD_COL10					(0x00) // Column 10

#define LCD_WOLF						(0)
#define LCD_ADC							(1)
#define LCD_RUN							(2)
#define LCD_SW1							(3)
#define LCD_SW2							(4)
#define LCD_FOUND					(5)
#define LCD_ERROR					(6)
#define LCD_MAIN						(7)
#define LCD_BAUD						(8)
#define LCD_TRAN						(9)
#define LCD_RECV						(10)
#define LCD_CLR							(11)
#define LCD_IP							(12)
#define LCD_CMD							(13)
#define LCD_SEEK						(14)
#define LCD_CHKPT					(15)
#define LCD_FOLLOW					(16)
#define LCD_EXIT						(17)
#define LCD_STOP						(18)
#define LCD_CIRCLE					(20)
#define LCD_START					(21)
#define LCD_ALIGN					(22)
#define LCD_CHAR_ZERO			('0')

//GPIO STATES
#define GPIO_LOW							(0x00) // Logic 0
#define GPIO_HIGH						(0x01)	//Logic 1

//Port 1 Pins
#define RED_LED 						(0x01) // 0 RED LED 0
#define A1_SEEED						(0x02) // 1 A1_SEEED
#define V_DETECT_L					(0x04) // 2 V_DETECT_L
#define V_DETECT_R					(0x08) // 3 V_DETECT_R
#define A4_SEEED						(0x10) // 4 A4_SEEED
#define V_THUMB							(0x20) // 5 V_THUMB
#define UCA0RXD							(0x40) // 6 Back Channel UCA0RXD
#define UCA0TXD							(0x80) // 7 Back Channel UCA0TXD

//Port 2 Pins
#define RESET_LCD					(0x01) // 0 RESET LCD
#define Check_Bat					(0x02) // 1 Check ADC Voltages
#define IR_LED							(0x04) // 2 IR_LED
#define SW2									(0x08) // 3 SW2
#define IOT_RUN_CPU				(0x10) // 4 IOT_RUN_CPU
#define DAC_ENB							(0x20) // 5 DAC_ENB
#define LFXOUT							(0x40) // 6 XOUTR
#define LFXIN								(0x80) // 7 XINR

//Port 3 Pins
#define TEST_PROBE					(0x01) // 0 TEST PROBE
#define OA2O								(0x02) // 1 OA2O
#define OA2N								(0x04) // 2 OA2N
#define OA2P								(0x08) // 3 OA2P
#define SMCLK								(0x10) // 4 SMCLK
#define DAC_CNTL						(0x20) // 5 DAC signal from Processor
#define IOT_LINK_CPU				(0x40) // 6 IOT_LINK_CPU
#define IOT_EN_CPU					(0x80) // 7 IOT_EN_CPU
#define USE_GPIO						(0x00)
#define USE_SMCLK					(0x01)

//Port 4 Pins
#define DAC_CNTL1					(0x01) // 0 DAC_CNTR1
#define SW1									(0x02) // 1 SW1
#define UCA1RXD							(0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD							(0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD				(0x10) // 4 Chip Select
#define UCB1CLK							(0x20) // 5 SPI mode - clock output—UCB1CLK
#define UCB1SIMO						(0x40) // 6 UCB1SIMO
#define UCB1SOMI						(0x80) // 7 UCB1SOMI

//Port 5 Pins
#define CHECK_BAT					(0x01) // 0 CHECK_BAT
#define V_BAT								(0x02) // 1 V_BAT
#define V_DAC								(0x04) // 2 V_DAC
#define V_3_3								(0x08) // 3 V_3_3
#define IOT_BOOT_CPU				(0x10) // 4 IOT_BOOT_CPU

//Port 6 Pins
#define R_FORWARD					(0x01) // 0 R_FORWARD
#define R_REVERSE					(0x02) // 1 R_REVERSE
#define L_FORWARD					(0x04) // 2 L_FORWARD
#define L_REVERSE					(0x08) // 3 L_REVERSE
#define LCD_BACKLITE				(0x10) // 4 LCD_BACKLIGHT
#define P6_5								(0x20) // 5 P6_5
#define GRN_LED							(0x40) // 6 GRN_LED

//Run States
#define IDLE								(0)
#define SEEK								(1)
#define ALIGN								(2)
#define FOLLOW							(3)
#define MANUAL							(4)
#define SET_UP							(5)
#define EXIT_CIRCLE				(6)
#define STOP_COURSE				(7)

//Wheel States
#define WHEELS_OFF					(0)
#define FORWARD							(1)
#define REVERSE							(2)
#define R_TURN							(3)
#define L_TURN							(4)
#define R_TURN_WIDE				(5)
#define L_TURN_WIDE				(6)
#define R_TURN_SHARP				(7)
#define L_TURN_SHARP				(8)
#define ZERO_RIGHT					(9)
#define ZERO_LEFT					(10)
#define RIGHT_OFFSET				(200)
#define LEFT_OFFSET				(0)

//PWM
#define WHEEL_PERIOD				(50000)
#define WHEEL_OFF					(0)
#define WHEEL_FULL					(50000)
#define WHEEL_HALF					(25000)
#define WHEEL_SLOW					(10000)
#define WHEEL_VSLOW				(7500)
#define WHEEL_CRAWL				(6500)
#define WHEEL_FAST					(35000)
#define R_FWD_SPEED				(TB3CCR1)
#define R_REV_SPEED				(TB3CCR2)
#define L_FWD_SPEED				(TB3CCR3)
#define L_REV_SPEED				(TB3CCR4)

//Timers
#define WAIT_TIME_0					(0)
#define WAIT_TIME_5					(5)
#define WAIT_TIME_10					(10)
#define WAIT_TIME_25					(25)
#define WAIT_TIME_40					(40)
#define WAIT_TIME_50					(50)
#define WAIT_TIME_60					(60)
#define WAIT_TIME_75					(75)
#define WAIT_TIME_100				(100)
#define WAIT_TIME_150				(150)
#define WAIT_TIME_500				(500)
#define CYCLE_5								(5)
#define CYCLE_10							(10)
#define CYCLE_25							(25)
#define CYCLE_50							(50)
#define CYCLE_100						(100)
#define CYCLE_250						(250)
#define CYCLE_500						(500)
#define CYCLE_1000						(1000)

//Clock Divs
#define DIVS 								(0x0030)
#define DIVS_L 							(0x0030)
#define DIVS0 							(0x0010)
#define DIVS0_L 						(0x0010)
#define DIVS1 							(0x0020)
#define DIVS1_L 						(0x0020)
#define DIVS_0 							(0x0000) // /1
#define DIVS_1 							(0x0010) // /2
#define DIVS_1_L 						(0x0010)
#define DIVS_2 							(0x0020) // /4
#define DIVS_2_L 						(0x0020)
#define DIVS_3 							(0x0030) // /8
#define DIVS_3_L 						(0x0030)
#define DIVS__1 						(0x0000) // /1
#define DIVS__2 						(0x0010) // /2
#define DIVS__2_L 					(0x0010)
#define DIVS__4 						(0x0020) // /4
#define DIVS__4_L 					(0x0020)
#define DIVS__8 						(0x0030) // /8
#define DIVS__8_L 					(0x0030)

//timers
#define TB0CCR0_INTERVAL     (2500)  // 5MS on SMCLK
#define TB0CCR1_INTERVAL 		(50000) // 0.1 seconds on SMCLK
#define TB0CCR2_INTERVAL			(50000) // 100 MS on SMCLK
#define TB1CCR0_INTERVAL			(410)  // 1 second on ACLK
#define SEEK_INTERVAL					(120)		// 12 second seek timer
#define WAIT_INTERVAL					(2)			// 200 ms timer before switching F & R
#define FOUND_INTERVAL				(30)		// 3 second wait timer after line found
#define ALIGN_INTERVAL				(40)

//Delay for TB0CCR0
#define DELAY_50MS							(10)
#define DELAY_200MS						(40)
#define DELAY_500MS						(100) 

//ADC Defines
#define CH_LEFT										(0)
#define CH_RIGHT									(1)
#define CH_THUMB									(2)
#define ASCII_OFFSET							(48)
#define TIMER_MULTIPLIER					(5)
#define L_OFF											(2900)
#define R_OFF											(2900)
#define ON_WHITE									(1800)
#define RIGHT											('R')
#define LEFT											('L')
#define NONE											('N')
#define TEN_MULTIPLIER						(10)

//Serial Comm
#define RNG_SIZE									(48)
#define PROC_SIZE								(48)
#define START											(0)
#define BAUD_115200							(115)
#define BAUD_460800							(460)
#define BUFF_IP_SIZE							(16)
#define TMP_IP_SIZE							(10)

//Start Up IoT Delays
#define IOT_1											(20)
#define IOT_2											(40)
#define IOT_3											(60)
#define IOT_4											(80)
#define IOT_5											(120)

//Serial Commands and IP
#define COM_CHAR									('^')
#define ITTALICS									('"')
#define COM_FORWARD								('F')
#define	COM_REVERSE								('B')
#define COM_RIGHT									('R')
#define COM_LEFT									('L')
#define COM_ZERO_PT								('Z')
#define COM_SEEK									('S')
#define COM_EXIT									('E')
#define COM_SLOW									('S')
#define COM_HALF									('H')
#define COM_FAST									('F')
#define COM_CHECK_PT							('C')
#define COM_MINUS									('-')
#define COM_PLUS									('+')
#define COM_NUM										(2)
#define COM_SPEED									(3)
#define COM_TIME									(4)
#define	COM_PW1										(0)
#define COM_PW2										(1)
#define PW1_CHAR									('8')
#define PW2_CHAR									('6')
#define COM_HALT									('H')
#define COM_ADC										('A')
#define COM_HELLO								('H')
#define COM_INIT									('I')
#define COM_IOT_RESET						('9')
#define IP_DOT										('.')

