//#########################################################################
//# eps_cc1_lib.h
//#
//# Autor: 	Christian Faustmann
//# Function:	EPS - Communication Controller 1 - Function Library Header
//#
//# VERSION HISTORY:
//#
//# - 26.03.2015 created
//# - 26.03.2015 included MCTtimer- & MeasureTimer prototypes
//# - 03.04.2015 FlightMode added
//# - 05.05.2015 I2C-TTC Clock&Data added
//#
//#########################################################################

#ifndef EPS_CC1_LIB_H_
#define EPS_CC1_LIB_H_

//#########################################################################
//		Includes
//#########################################################################
#include "eps_cc1.h"

//#########################################################################
//		Prototypes
//#########################################################################
//	eps_cc1_lib.h:
//		MCTimer
void resetMCTimer(unsigned int delay);						//reset function for MCTimer (eps_cc1_lib.h)
inline void enableMCTimerISR(void);					//enable MCTimer interrupt (eps_cc1_lib.h)
inline void disableMCTimerISR(void);				//disable MCTimer Interrupt (eps_cc1_lib.h)
inline void clearMCTimerIFLG(void);					//clear MCTimer interrupt flag (eps_cc1_lib.h)
//		MeasureTimer
void resetMeasureTimer(unsigned int delay);			//reset function for MeasureTimer (eps_cc1_lib.h)
inline void enableMeasureTimerISR(void);			//enable MeasureTimer interrupt (eps_cc1_lib.h)
inline void disableMeasureTimerISR(void);			//disable MeasureTimer interrupt (eps_cc1_lib.h)
inline void clearMeasureTimerIFLG(void);			//clear MeasureTimer interrupt flag (eps_cc1_lib.h)
//		Outputs and Status
void SetOutputs(void);								//Update Outputs (eps_cc1_lib.h)
void UpdateStatus(void);							//Update status with Pin values (eps_cc1_lib.h)
// 		ADC
inline unsigned char readVcc(void);					//read Vcc with interal ADC (eps_cc1_lib.h)
inline unsigned char readTemp(void);				//read the temperature with interal ADC (eps_cc1_lib.h)
//		I2C-TTC Clock
inline void enableClockISR(void);					//enable P1.4 interrupt (eps_cc1_lib.h)
inline void disableClockISR(void);					//disable P1.4 interrupt (eps_cc1_lib.h)
inline void HiLoClockEdge(void);					//set P1.4 interrupt to High-Low Edge active (eps_cc1_lib.h)
inline void LoHiClockEdge(void);					//set P1.4 interrupt to Low-High Edge active (eps_cc1_lib.h)
inline void ClockAsInput(void);						//set P1.4 as Input (normal mode) (eps_cc1_lib.h)
//		I2C-TTC Data
inline void enableDataISR(void);					//enable P1.5 interrupt (eps_cc1_lib.h)
inline void disableDataISR(void);					//disable P1.5 interrupt (eps_cc1_lib.h)
inline void HiLoDataEdge(void);						//set P1.5 interrupt to High-Low Edge active (eps_cc1_lib.h)
inline void LoHiDataEdge(void);						//set P1.5 interrupt to Low-High Edge active (eps_cc1_lib.h)
inline void DataAsOutput(void);						//set P1.5 as Output (for acknowlegde) (eps_cc1_lib.h)
inline void DataAsOutputHigh(void);					//set P1.5 as Output High
inline void DataAsInput(void);						//set P1.5 as Input (normal mode) (eps_cc1_lib.h)
inline void i2cWatchdog(void);						//Clear I2C if running to long
inline void buildRspVector(void);
inline void SetOutputs(void);						//sets the outputs according to the force value registers and the internal requests

// I2C
inline void i2cObcReset(void);						//Reset Hardware I2C

//	eps_cc1_powerup.c:
void CC1Powerup(void);								//PowerUp Funktion (eps_cc1_powerup.c)

//	eps_cc1_flightmode.c:
void CC1FlightMode(void); 							//Flight mode function (eps_cc1_flightmode.c)

void CC1SafeMode(void);


#endif /* EPS_CC1_LIB_H_ */
