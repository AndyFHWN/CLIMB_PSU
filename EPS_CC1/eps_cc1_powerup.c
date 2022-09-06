//#########################################################################
//# eps_cc1_powerup.c
//#
//# Autor: 	Christian Faustmann, Thomas Riel
//# Function:	EPS - Communication Controller 1 - PowerUp Mode
//#
//# VERSION HISTORY:
//#
//# - 26.03.2015 created
//# - 26.03.2015 MCTimer & MeasureTimer included
//#
//#########################################################################


//#########################################################################
//		Includes
//#########################################################################
#include <ti/mcu/msp430/Grace.h>			//Grace related includes
#include "eps_cc1.h"						//EPS_CC1 global Header
#include "eps_cc1_lib.h"					//EPS_CC1 function library
#include "eps_cc1_globals.h"					//EPS_CC1 globals

void CC1Powerup(void)
{
	Grace_init();                   		//activate Grace-generated configuration

	//init_MCTimer
	resetMCTimer(TIMER_20S);				//initialise MCTimer to 30s
	enableMCTimerISR();						//activate MCTimer interrupt

	//init_MeasureTimer
	resetMeasureTimer(TIMER_10S);			//set Measure delay to 10s
	enableMeasureTimerISR();				//activate MeasureTimer interrupt

	//init_I2C-TTC
	//Data Pin
	DataAsInput();							//set Data Pin as input
	HiLoDataEdge();							//interrupt Hi-Lo activ
	enableDataISR();						//enable ISR

	//Clock Pin
	ClockAsInput();							//set Clock Pin as input
	LoHiClockEdge();						//interrupt Lo-Hi activ
	disableClockISR();						//enable ISR

	DATA_REG[40]= SOFTWARE_VERSION;			//set Software Version of CC1
	DATA_REG[62]= 0xAC;						//set ID of CC1

	//status = 0bXXXX EN_I2C EN_BAT1 EN_BAT2 EN_3V3_BACKUP
	OSR = OSR_EN_I2C | //(((status&EN_I2C)>>EN_I2C)<<OSR_EN_I2C) |
		  (((status&0x04)>>2)<<1) |
		  (((status&0x02)>>1)<<2) |
		  (((status&0x01)>>0)<<3);

	DATA_REG[60] = status;

	if (status&0x40)
	{
		CC1Mode = CC1_FLIGHT;
	}
	else
	{
		CC1Mode = CC1_SAFE;
	}

	unsigned char k = 0;
	for(k = 0; k<=8; k++)
	{
		DATA_REG[64+k] = 0x01;				//init registers with odd parity
	}
	initFlag = 1;

	__bis_SR_register(GIE);       			//interrupt enable

	// if first boot up then wait for 35min or until MC data is received
    if ((bootcounter == 0) && (startUpCnt < 210))
    {
    	CC1Mode = CC1_BOOT;

		// disable Bat1, disable Bat2, 3V3_Backup off
		// I2C connected
		OSR &= ~(OSR_EN_BAT1 | OSR_EN_BAT2 | OSR_EN_3V3_BACKUP);
		OSR |= OSR_EN_I2C;
		SetOutputs();						//Check the Force Value Registers
		UpdateStatus();						//Update the status register

 /*   	while (startUpCnt < STARTUP_DELAY) // 35min
    	{
    		WDT_A_resetTimer(WDT_A_BASE);		// Clear watchdog timer counter

    		_BIS_SR(LPM2_bits + GIE); 			// Enter LPM2 w/interrupt

    		if (CC1Measure == 1)
    		{
    			startUpCnt++;
    			resetMeasureTimer(TIMER_10S);	//Reset measure Timer to 10s
    		}
    	}
*/  }
    else
    {
		SetOutputs();
    }

    // increment bootcounter and set startup counter to finish
    bootcounter++;
    startUpCnt = STARTUP_DELAY;

    // check if cross power supply was active prior to reset
	if (crossPowerStatus >= CROSS_POWER_ON_CNT) // cross power supply was just switched on befor reset and MC did not respond
	{
		crossPowerStatus = CROSS_POWER_OFF_CNT;	// disable cross power supply
	}

}



