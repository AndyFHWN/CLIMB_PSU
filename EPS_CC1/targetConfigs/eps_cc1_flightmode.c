//#########################################################################
//# eps_cc1_flightmode.c
//#
//# Autor: 	Christian Faustmann
//# Function:	EPS - Communication Controller 1 - Flight Mode
//#
//# VERSION HISTORY:
//#
//# - 03.04.2015 created
//#
//#########################################################################


//#########################################################################
//		Includes
//#########################################################################
#include "eps_cc1.h"							//EPS_CC1 header
#include "eps_cc1_lib.h"						//EPS_CC1 function library
#include "eps_cc1_globals.h"					//EPS_CC1 globals
#include <driverlib/MSP430FR57xx/adc10_b.h>		//ADC include
#include <driverlib/MSP430FR57xx/eusci_a_uart.h>

//#########################################################################
//		Prototypes
//#########################################################################


//#########################################################################
//		CC1FlightMode
//#########################################################################
void CC1FlightMode(void)
{
	// check if mode has just changed
	if(CC1ModeChanged != 0)
	{
		// disable Bat1
		// disable Bat2
		// 3V3_Backup off
		// I2C connected
		OSR &= ~(OSR_EN_BAT1 | OSR_EN_BAT2 | OSR_EN_3V3_BACKUP);
		OSR |= OSR_EN_I2C;

		enableMCTimerISR();				//Enable MC Timer
		resetMCTimer(TIMER_20S);		//reset Timer to 20s

		CC1ModeChanged = 0;
	}

	// check if message from MC is available
	if(CC1RspPending != 0)
	{
		buildRspVector();				//build response and send first byte
		UART_TXDATA = RspVector[0];		//send first byte of response
		CC1RspPending = 0;
		crossPowerStatus = 0;			//reset cross power supply status
	}

	// measure Vcc and temperature
	if(CC1Measure != 0)
	{
		// i2cWatchdog();

		//measure Vcc
		readVcc();

		//measure Temp
		readTemp();

		// restart measure timer
		resetMeasureTimer(TIMER_10S);	//Reset measure Timer to 10s
	}

	// if MC has not responded for the MC timeout time, change to safe mode
	if(MCTimeout != 0)
	{
		//MCTimeout = 0;
		disableMCTimerISR();				//disable MCTimer as long as in safe mode
		clearMCTimerIFLG();					//clear timer interrupt flag
		CC1Mode = CC1_SAFE;					//switch to safe mode
		CC1ModeChanged = 1;					//set mode change flag
	}

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

	WDT_A_resetTimer(WDT_A_BASE);			// Clear watchdog timer counter

	if((CC1Mode == CC1_FLIGHT) && (i2cTtcRunning == 0))
	{
		_BIS_SR(LPM2_bits + GIE); 			// Enter LPM2 w/interrupt
	}


}


//#########################################################################
//		Subroutines
//#########################################################################


