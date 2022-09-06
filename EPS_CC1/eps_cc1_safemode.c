//#########################################################################
//# eps_cc1_safemode.c
//#
//# Autor: 	Thomas Riel
//# Function:	EPS - Communication Controller 1 - Safe Mode
//#
//# VERSION HISTORY:
//#
//# - 17.05.2015 created
//#
//#########################################################################


//#########################################################################
//		Includes
//#########################################################################
#include "eps_cc1.h"							//EPS_CC1 header
#include "eps_cc1_lib.h"						//EPS_CC1 function library
#include "eps_cc1_globals.h"					//EPS_CC1 globals
#include <driverlib/MSP430FR57xx/eusci_a_uart.h>

//#########################################################################
//		Prototypes
//#########################################################################

void buildRspVector(void);

//#########################################################################
//		CC1SafeMode
//#########################################################################
void CC1SafeMode(void)
{
	// check if mode has just changed
	if(CC1ModeChanged != 0)
	{
		// enable Bat1
		// enable Bat2
		// 3V3_Backup on
		// I2C connected
		OSR |= OSR_EN_BAT1 | OSR_EN_BAT2 | OSR_EN_3V3_BACKUP | OSR_EN_I2C;

		CC1ModeChanged = 0;
	}

	// check if message from MC is available
	if(CC1RspPending != 0)
	{
		MCTimeout = 0;
		buildRspVector();					//build response and send first byte
		UART_TXDATA = RspVector[0];			//send first byte of response
		CC1RspPending = 0;
		CC1Mode = CC1_FLIGHT;				//switch to flight mode
		CC1ModeChanged = 1;					//set mode change flag
		crossPowerStatus = 0;				//reset cross power supply status
	}
	else
	{
		if (crossPowerStatus >= CROSS_POWER_OFF_CNT)
		{
			P1DIR &= ~(EN_CROSS_POWER_SUPPLY);	// cross power supply pins as inputs
			P1OUT &= ~(EN_CROSS_POWER_SUPPLY);	// disable cross power supply
		}
		else
		{
			crossPowerStatus++;
			if (crossPowerStatus >= CROSS_POWER_ON_CNT)
			{
				P1DIR |= EN_CROSS_POWER_SUPPLY;		// cross power supply pins as outputs
				P1OUT |= (EN_CROSS_POWER_SUPPLY);	// enable cross power supply
			}
		}

	}

	// measure Vcc and Temperature for battery management
	if(CC1Measure != 0)
	{
		// i2cWatchdog();

		//measure Vcc and see if it is valid
		if(readVcc() == 1)
		{
			// Battery Voltage Management
			if ((DATA_REG[56] < VCC_MIN) || (DATA_REG[56] > VCC_MAX) || ((DATA_REG[49]&0x83) == 0x83)) //if voltage out of bound or MC in power down mode
			{
				// disconnect both batteries and disable 3V3-Backup
				OSR &= ~(OSR_EN_BAT1 | OSR_EN_BAT2 | OSR_EN_3V3_BACKUP);

				// change MC status 2 as batteries are now not safely connected
				DATA_REG[49] &= ~0x60;
			}
			else
			{
				// otherwise connect both batteries and enable 3V3-Backup
				OSR |= (OSR_EN_BAT1 | OSR_EN_BAT2 | OSR_EN_3V3_BACKUP);
			}
			SetOutputs();
		}

		//measure Temp
		readTemp();

		resetMeasureTimer(TIMER_10S);	//Reset measure Timer to 10s
	}

	// i2c watchdog
	if(i2cObcRunning != 0)
	{
		//Count how long the I2C is running to detect hanging
		i2cObcRunning++;
		if(i2cObcRunning >= 200)
		{
			//I2C is hanging
			i2cObcReset(); 	//Reset hardware I2C
		}
	}

	if ((i2cTtcRunning == 0) && ( ((P1IN & BIT5) == 0 ) || ((P1IN & BIT4) == 0 ) ))
	{
		// if i2c of ttc is not running but SDA or SCL is low -> reset i2c
		ClockAsInput();				//both pins as Input
		DataAsInput();

		LoHiClockEdge();
		HiLoDataEdge();				//set ISR for start condition

		enableClockISR();
		enableDataISR();

		i2cTtcRunning = 0;

		enableMeasureTimerISR();			//enable Measure Timer
		resetMeasureTimer(TIMER_10S);	//Reset measure Timer to 10s

#if DISABLE_INTERRUPTS_ON_I2C
		enableMCTimerISR();
		EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
		EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
#endif
	}
	WDT_A_resetTimer(WDT_A_BASE);		// Clear watchdog timer counter

	if((CC1Mode == CC1_SAFE) && (CC1ModeChanged == 0) && (i2cTtcRunning == 0))
	{
		_BIS_SR(LPM2_bits + GIE); 		// Enter LPM2 w/interrupt
	}
}

//#########################################################################
//		Subroutines
//#########################################################################
