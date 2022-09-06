//#########################################################################
//# main.c
//#
//# Autor: 	Christian Faustmann, Thomas Riel
//# Function:	EPS - Communication Controller 1
//#
//# VERSION HISTORY:
//#
//# - 05.02.2015 created
//# - 29.03.2015 Pin assignment update
//# - 03.04.2015 increase compiler optimization to level 3 (Properties/Build/MSP430 Compiler/Optimiziation
//# - 04.04.2015 add RspVector[31]
//# - 09.04.2015 pin & register initialisation
//#
//#########################################################################
//# CONNECTIONS CC1:
//# P1.4 - I2C-SCL_TC2 ... I2C clock to Telemetry and Telecommand Module(TT&C) (IN w/ pullup)
//# P1.5 - I2C-SCA_TC2 ... I2C data to Telemetry and Telecommand Module(TT&C) (IN w/ pullup)
//# P1.6 - SDA1_SW ... I2C data to On Board Computer(OBC)
//# P1.7 - SCL1_SW ... I2C clock to On Board Computer(OBC)
//#
//# P2.0 - UC_TX1 ... UART transiver pin
//# P2.1 - UC_RX1 ... UART receiver pin
//# P2.2 - EN_I2C1 ... Enable I2C switch (OUT high active)
//#
//# PJ.0 - EN_FET_1-1-3	... Bat1 (OUT high active)
//# PJ.1 - EN_FET-2-1-2 ... Bat2 (OUT high active)
//# PJ.2 - EN_3V3_BACKUP_CC1 ... 3V3 BACKUP Switch (OUT high active)
//# PJ.3 - RBF_INT ... Remove befor flight (IN w/ pullup)
//#
//#
//#
//#
//# Inbetriebnahme: Stecker senkrecht rechts
//# TEST oben / RST unten
//#########################################################################



//#########################################################################
//		EPS CC1 includes
//#########################################################################
#include "eps_cc1.h"
#include "eps_cc1_lib.h"
#include "global_vars.h"
#include "eps_cc1_globals.h"
#include <driverlib/MSP430FR5xx_6xx\timer_a.h>



//#########################################################################
// 		main
//#########################################################################
void main(void)
{
	WDT_A_resetTimer(WDT_A_BASE);			// Clear watchdog timer counter

    CC1Powerup();							//CC1 Initialisation
    //CC1ModeChanged = 0;					//to run initialization of mode

    while(1)
    {
    	WDT_A_resetTimer(WDT_A_BASE);		//Clear watchdog timer counter

    	SetOutputs();						//Check the Force Value Registers

    	UpdateStatus();						//Update the status register

    	if(CC1Mode == CC1_FLIGHT)
    	{
    		CC1FlightMode();				//Execute Flight Mode
    	}
    	else
    	{
    		CC1SafeMode();					//Execute Safe Mode
    	}
    }
}




