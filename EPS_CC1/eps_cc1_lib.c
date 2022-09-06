//#########################################################################
//# eps_cc1_lib.h
//#
//# Autor: 	Christian Faustmann
//# Function:	EPS - Communication Controller 1 - Function Library
//#
//# VERSION HISTORY:
//#
//# - 26.03.2015 created
//# - 26.03.2015 included MCTtimer- & MeasureTimer functions
//# - 03.04.2015 declare some functions as inline
//# - 05.05.2015 I2C-OBC Clock&Data functions
//#
//#########################################################################



//#########################################################################
//		Includes
//#########################################################################
#include <ti/mcu/msp430/Grace.h>				//Grace related includes
#include "src/grace/_Grace.h"					//for I2C reinitialization
#include "eps_cc1_lib.h"
#include "eps_cc1_globals.h"					//EPS_CC1 globals
#include <driverlib/MSP430FR57xx/eusci_a_uart.h>

//#########################################################################
//		MCTimer(Timer0)
//#########################################################################
//inline void resetMCTimer(void)							//reset function for MCTimer
void resetMCTimer(unsigned int delay)				//reset function for MeasureTimer(delay -> 10375 = 10s)
{
	MCTimeout = 0;									//reset MC timeout
	TA0CCR0 = delay; 								//set timer counter reg to 20s
}

inline void enableMCTimerISR(void)						//enable MCTimer Interrupt
{
	TA0CCTL0 |= CCIE;
}

inline void disableMCTimerISR(void)						//disable MCTimer Interrupt
{
	TA0CCTL0 &= ~CCIE;
}

inline void clearMCTimerIFLG(void)						//Clear Timer0 interrupt flag
{
	TA0CTL &= ~TAIFG;
}



//#########################################################################
//		MeasureTimer(Timer1)
//#########################################################################
void resetMeasureTimer(unsigned int delay)				//reset function for MeasureTimer(delay -> 10375 = 10s)
{
	CC1Measure = 0;
	TA1CCR0 = delay;
}

inline void enableMeasureTimerISR()						//enable Measure Timer
{
	TA1CCTL0 |= CCIE;									//Timer1 interrupt enabled
}

inline void disableMeasureTimerISR(void)				//disable Measure Timer Interrupt
{
	TA1CCTL0 &= ~CCIE;
}

inline void clearMeasureTimerIFLG(void)					//Clear Timer1 interrupt flag
{
	TA1CTL &= ~TAIFG;
}

//#########################################################################
//		I2C watchdog
//#########################################################################
inline void i2cWatchdog(void)					//Clear I2C if running to long
{
	if (i2cTtcRunning != 0)
	{
		ClockAsInput();
		DataAsInput();

		LoHiClockEdge();
		HiLoDataEdge();				//set ISR for start condition

		enableClockISR();
		enableDataISR();

		i2cTtcRunning = 0;

		enableMeasureTimerISR();		//enable Measure Timer
		resetMeasureTimer(TIMER_10S);	//Reset measure Timer to 10s

#if DISABLE_INTERRUPTS_ON_I2C
		enableMCTimerISR();
		EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
		EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
#endif
	}
}

inline void i2cObcReset(void)			//Reset Hardware I2C
{
	UCB0CTLW0 |= UCSWRST;				//Software reset the I2C interface
	EUSCI_B0_graceInit();				//reinit I2C
	UCB0CTLW0 &= ~UCSWRST;				//release again I2C interface
	i2cObcRunning = 0;					//I2C is not running
}

//#########################################################################
//		ADC
//#########################################################################
// reads the supply voltage of the uC and returns 1 when result is valid
inline unsigned char readVcc(void)
{
	CC1Measure = 0;

	// Set ADC channel to Vcc/2
	ADC10CTL0 &= ~ADC10ENC;
	ADC10MCTL0 = (ADC10MCTL0 & 0xFFF0) | ADC_MUX_VCC;

	// Enable the ADC10_B module
	ADC10_B_enable(ADC10_B_BASE);

	// Enable and Start the conversion in Single-Channel, Single Conversion Mode
	ADC10_B_startConversion(ADC10_B_BASE, ADC10_B_SINGLECHANNEL);

	resetMeasureTimer(TIMER_500MS);	//Reset measure Timer to 500ms

	while ((ADC10_B_isBusy(ADC10_B_BASE) != 0) && (CC1Measure == 0));

	// Disable the ADC10_B module
	ADC10_B_disable(ADC10_B_BASE);

	if (CC1Measure == 0)
	{
		// Store ADC10_B conversion result into the register
		DATA_REG[56] = ((ADC10_B_getResults(ADC10_B_BASE))*80)/512;

		// Validate the ADC reading
		if ((DATA_REG[56] < VCC_MAX_VALID) && (DATA_REG[56] > VCC_MIN_VALID))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

// reads the temperature of the uC and returns 1 when result is valid
inline unsigned char readTemp(void)
{
	CC1Measure = 0;

	// set adc channel to temp sensor
	ADC10CTL0 &= ~ADC10ENC;
	ADC10MCTL0 = (ADC10MCTL0 & 0xFFF0) | ADC_MUX_TEMP;

	// Enable the ADC10_B module
	ADC10_B_enable(ADC10_B_BASE);

	// Enable and Start the conversion in Single-Channel, Single Conversion Mode
	ADC10_B_startConversion(ADC10_B_BASE, ADC10_B_SINGLECHANNEL);

	resetMeasureTimer(TIMER_500MS);	//Reset measure Timer to 500ms

	while ((ADC10_B_isBusy(ADC10_B_BASE) != 0) && (CC1Measure == 0));

	// Disable the ADC10_B module
	ADC10_B_disable(ADC10_B_BASE);

	if (CC1Measure == 0)
	{
		// Store ADC10_B conversion result into the register
		// convert to Voltage UFix 1.7,
		DATA_REG[57] = ((((ADC10_B_getResults(ADC10_B_BASE))*160)/512)-109)*3+25;

		// Validate the ADC reading
		if ( (((char)DATA_REG[57]) < TEMP_MAX_VALID) && (((char)DATA_REG[57]) > TEMP_MIN_VALID) )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
//#########################################################################
//		Outputs and Status
//#########################################################################
// updates the status register
inline void UpdateStatus( void )
{
	// Update CC Mode
	if(CC1Mode == CC1_FLIGHT)
	{
		DATA_REG[60] = 0x40;
	}
	else if(CC1Mode == CC1_SAFE)
	{
		DATA_REG[60] = 0x80;
	}
	else
	{
		DATA_REG[60] = 0x00;
	}

	// Update MC Timeout
	if(MCTimeout != 0)
	{
		DATA_REG[60] |= 0x20;
	}

	// Update EN_I2C
	if(P2IN & EN_I2C)
	{
		DATA_REG[60] |= 0x08;
	}

	// Update EN_BAT1
	if(PJIN & EN_BAT1)
	{
		DATA_REG[60] |= 0x04;
	}

	// Update EN_BAT1
	if(PJIN & EN_BAT2)
	{
		DATA_REG[60] |= 0x02;
	}

	// Update EN_3V3_BACKUP
	if(PJIN & EN_3V3_BACKUP)
	{
		DATA_REG[60] |= 0x01;
	}

	status = DATA_REG[60];
	DATA_REG[54] = bootcounter;
}

// sets the outputs according to the force value register and the internal requests
inline void SetOutputs( void )
{
	//# OSR: B7-B4: TBD, B3: EN_3V3_BACKUP, B2: EN_FET-2-2-2, B1: EN_FET-1-1-3, B0 (LSB): EN_I2C1
	//# P2.2 - EN_I2C1 ... Enable I2C switch (OUT high active)
	//# PJ.0 - EN_FET_1-1-3	... Bat1 (OUT high active)
	//# PJ.1 - EN_FET-2-2-2 ... Bat2 (OUT high active)
	//# PJ.2 - EN_3V3_BACKUP_CC1 ... 3V3 BACKUP Switch (OUT high active)

	// Update Output Status register with Force Value Register
	OSR = (OSR & ~DATA_REG[79]) | (DATA_REG[79] & DATA_REG[80]);
	//DATA_REG[63] = OSR;

	//set EN_3V3_Backup
	if(OSR & OSR_EN_3V3_BACKUP)
	{
		PJOUT |= (EN_3V3_BACKUP);
	}
	else
	{
		PJOUT &= ~(EN_3V3_BACKUP);
	}

	//set EN_BAT2
	if(OSR & OSR_EN_BAT2)
	{
		PJOUT |= (EN_BAT2);

		// change MC status 2 as bat2 is now connected
		DATA_REG[49] |= 0x40;
	}
	else
	{
		PJOUT &= ~(EN_BAT2);
	}

	//set EN_BAT1
	if(OSR & OSR_EN_BAT1)
	{
		PJOUT |= (EN_BAT1);

		// change MC status 2 as bat1 is now connected
		DATA_REG[49] |= 0x20;
	}
	else
	{
		PJOUT &= ~(EN_BAT1);
	}

	//set EN_I2C1
	if(OSR & OSR_EN_I2C)
	{
		P2OUT |= (EN_I2C);
	}
	else
	{
		P2OUT &= ~(EN_I2C);
	}
}

//#########################################################################
//		I2C-TTC P1.4 (Clock)
//#########################################################################
inline void enableClockISR(void)						//enable P1.4 interrupt
{
	P1IE |= BIT4;
}

inline void disableClockISR(void)						//disable P1.4 interrupt
{
	P1IE &= ~BIT4;
}

inline void HiLoClockEdge(void)							//set P1.4 interrupt to High-Low Edge active
{
	//P1IE &= ~BIT4;
	P1IES |= BIT4;
}

inline void LoHiClockEdge(void)							//set P1.4 interrupt to Low-High Edge active
{
	//P1IE &= ~BIT4;
	P1IES &= ~BIT4;
}

inline void ClockAsInput(void)							//set P1.4 as Input (normal mode)
{
	P1DIR &= ~BIT4;
	P1REN |= BIT4; 		//Pull-Up/-Down enabeld
	P1OUT |= BIT4;		//Pullup
}



//#########################################################################
//		I2C-TTC P1.5 (Data)
//#########################################################################
inline void enableDataISR(void)							//enable P1.5 interrupt
{
	P1IE |= BIT5;
}

inline void disableDataISR(void)							//disable P1.5 interrupt
{
	P1IE &= ~BIT5;
}

inline void HiLoDataEdge(void)							//set P1.5 interrupt to High-Low Edge active
{
	//P1IE &= ~BIT5;
	P1IES |= BIT5;
}

inline void LoHiDataEdge(void)							//set P1.5 interrupt to Low-High Edge active
{
	//P1IE &= ~BIT5;
	P1IES &= ~BIT5;
}

inline void DataAsOutput(void)							//set P1.5 as Output (for acknowlegde)
{
	P1DIR |= BIT5;
	P1OUT &= ~BIT5; 	//Set Output to "0"
}

inline void DataAsOutputHigh(void)							//set P1.5 as Output High
{
	P1DIR |= BIT5;
	P1OUT |= BIT5; 	//Set Output to "1"
}

inline void DataAsInput(void)							//set P1.5 as Input (normal mode)
{
	//while(P1IN & BIT4);
	P1DIR &= ~BIT5;
	P1REN |= BIT5; 		//Pull-Up/-Down enabeld
	P1OUT |= BIT5;		//Pullup
}

//#########################################################################
//		UART Response
//#########################################################################
inline void buildRspVector(void) //for additional Information look at declaration of RspVector
{
	unsigned char i;
	unsigned char TmpChecksum = 0;

	UCA0CTLW0 |= UCSWRST;					//reset the UART

	RspVector[0] = '$';
	RspVector[1] = 'A';
	TmpChecksum = '$';
	TmpChecksum ^= 'A';

	if ((DATA_REG[49] & 0x07 == 0x03) && (initFlag != 0x02))		// if MC is in power down mode the first time
	{
		// Clear the Repetition of command flag
		DATA_REG[87] = 0x00;

		for(i=2;i<=10;i++)
		{
			RspVector[i] = 0x01;			//clear all current requests (0 with parity)
			DATA_REG[i+62] = 0x01;
			TmpChecksum ^= RspVector[i];
		}
		for(i=11;i<=25;i++)
		{
			RspVector[i] = DATA_REG[i+62];	//start with DATA_REG[73]
			TmpChecksum ^= RspVector[i];
		}
		initFlag = 0x02;
	}
	else
	{
		for(i=2;i<=25;i++)
		{
			RspVector[i] = DATA_REG[i+62];	//start with DATA_REG[64]
			TmpChecksum ^= RspVector[i];
		}
		if (initFlag == 0x02)
		{
			initFlag = 0x01;
		}
	}
	RspVector[26] = DATA_REG[54];			//Reboot Counter CC1
	TmpChecksum ^= RspVector[26];

	RspVector[27] = DATA_REG[56];			//Vcc_CC1
	TmpChecksum ^= RspVector[27];

	RspVector[28] = DATA_REG[57];			//Temp_CC1
	TmpChecksum ^= RspVector[28];

	RspVector[29] = DATA_REG[60];			//Status CC1
	TmpChecksum ^= RspVector[29];

	RspVector[30] = TmpChecksum;

	// Set the Repetition of command flag
	if (DATA_REG[87] < 10)
	{
		DATA_REG[87]++;
	}
	else
	{
		DATA_REG[87] = 10;
	}

	UCA0CTLW0 &= ~(UCSWRST);				//reenable the UART
	UCA0IE |= (UCRXIE)|(UCTXIE);

	txVectorPos = 0;						//reset tx vector position
}
