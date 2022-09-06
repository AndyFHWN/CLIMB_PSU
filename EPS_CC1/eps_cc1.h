//#########################################################################
//# eps_cc1.h
//#
//# Autor: 	Christian Faustmann
//# Function:	EPS - Communication Controller 1
//#
//# VERSION HISTORY:
//#
//# - 05.02.2015 	created
//# - 02.04.2015 	UART defines
//# - 05.04.2015 	changed CS_END from 63 to 65
//# 				added OwnAddress
//#
//#########################################################################

#ifndef EPS_CC_H_
#define EPS_CC_H_

//#########################################################################
//		EPS_CC1 Includes
//#########################################################################
#include <msp430.h>
#include <driverlib/MSP430FR57xx/adc10_b.h>		//ADC include
#include <driverlib/MSP430FR5xx_6xx\wdt_a.h>	//watchdog timer

//#########################################################################
//		EPS_CC1 Testmode
//#########################################################################
#define UART_TST_MODE 0						// activate UART test mode

//#########################################################################
//		EPS_CC1 Defines
//#########################################################################
#define EN_BAT1			BIT0				// connect bat1
#define EN_BAT2			BIT1				// connect bat2
#define EN_3V3_BACKUP	BIT2				// enable 3V3 backup
#define EN_I2C			BIT2				// enable I2C
#define EN_CROSS_POWER_SUPPLY	(BIT1|BIT0)	// Cross power supply pins
#define OSR_EN_BAT1			BIT1			// Output Status register position of en bat1
#define OSR_EN_BAT2			BIT2			// Output Status register position of en bat2
#define OSR_EN_3V3_BACKUP	BIT3			// Output Status register position of en backup
#define OSR_EN_I2C			BIT0			// Output Status register position of en i2c
#define OSR_EN_CROSS_POWER	BIT4			// Output Status register position of en cross power supply

#define CC1_FLIGHT 1						//status flight mode
#define CC1_SAFE 0							//status safe mode
#define CC1_BOOT 2							//status Boot mode
#define TIMER_5MS 5							//Timer register value for 5ms delay
#define TIMER_10MS 11						//Timer register value for 10ms delay
#define TIMER_250MS 259						//Timer register value for 250ms delay
#define TIMER_500MS 518						//Timer register value for 500ms delay
#define TIMER_1S 1036						//Timer register value for 1s delay
#define TIMER_10S 10375						//Timer register value for 10s delay
#define TIMER_20S 20740						//Timer register value for 20s delay
#define TIMER_30S 31125						//Timer register value for 30s delay
#define TIMER_60S 62250						//Timer register value for 60s delay
#define REG_DATA_END 63						//Last element of the DATA Registers
#define REG_END 87							//Last element of the Registers
#define DATA_MAX 91							//maximum data length of Data Vector via MC UART communication (not REG_END!!)
#define CS_END 66							//Checksum end for UART RX - checksum will be created form byte 0 to 65
#define CS_END_CMD 90						//Checksum end for UART CMD RX - checksum will be created form byte 67 to 89
#define RSP_MAX 30							//Last array element of the response vector in UART TX
#define UART_RXDATA uartRxTemp				//receive register uart
#define UART_TXDATA UCA0TXBUF				//transive register uart
#define OWN_ADDRESS 0x55					//Own address for I2C communication

#define VCC_MAX_VALID 0b10011100			//maximal valid Vcc is 4.865V
#define VCC_MIN_VALID 0b01000000			//maximal valid Vcc is 2.000V
#define VCC_MIN 0b01011100					//minimal Vcc for battery connection is 2.875V
#define VCC_MAX 0b10000100					//maximal Vcc for battery connection is 4.125V

#define TEMP_MAX_VALID (125)				//maximal valid Temp is 125°C
#define TEMP_MIN_VALID (-55)				//minimal valid Temp is -55°C

#define ADC_MUX_VCC 0x000B					//ADC Channel for Vcc measurement
#define ADC_MUX_TEMP 0x000A					//ADC Channel for Temp measurement

#define CROSS_POWER_ON_CNT 6				//cycles of 10s after which the cross power supply is switched on
#define CROSS_POWER_OFF_CNT 10				//cycles of 10s after which the cross power supply is switched off if MC does not respond

#define STARTUP_DELAY 210					//time in 10s from firts bootup to normal operation
#define DISABLE_INTERRUPTS_ON_I2C 0			// disable all other interrupts while the I2C to the TTC is running

#define SOFTWARE_VERSION 107				// Software Version of the CC1
//#########################################################################
//		EPS_CC1 Function Declarations
//#########################################################################


#endif /* EPS_CC_H_ */
