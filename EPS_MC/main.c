//---------------------------------------------------------
// Title:		main.c
//
// Author:		Patrick Kappl
//
// Creation:	04.01.2015
//
// Description:	main() contains all initialisations and the
//				boot mode
//---------------------------------------------------------

#include "main.h"

int main(void)
{
	//wdt_enable(WDTO_2S);
	
	//--- POWER UP MODE ---------------------------------------------------------------------------
	OSCCAL = 0xBB;
	InitPins();
	UARTInit();									// activate UARTs and set Baud rate and frame format
	UART0DeInit();								// deactivate UARTs
	UART1DeInit();
	TWIInit();									// activate TWI and set bit rate
	
	// initialize all VI and temperature sensors
	InitVISensors();
	ConfigTempSensor(&global_tBat1);
	ConfigTempSensor(&global_tBat2);
	ConfigTempSensor(&global_tCenter);
	ConfigTempSensor(&global_tEdge);

	TWIDeInit();								// deactivate TWI
	InitEEPROMRingBuffer();						// get the correct addresses for the ring buffers
	
	
	//////////////////////////////////////////////////////////////////////////
	DebugMode();
	//////////////////////////////////////////////////////////////////////////

	if((PIN_RBF_INT & (1<<RBF_INT)) == 0)		// if RBF is pulled low
	{
		//--- DEBUG MODE --------------------------------------------------------------------------
		DebugMode();
	}
	else
	{
		//--- BOOT MODE ---------------------------------------------------------------------------
		wdt_enable(WDTO_2S);					// activate watchdog timer with a timeout of ~1.9s
		SetEERebootCount(GetEERebootCount()+1);	// increment number of boots in EEPROM
		LoadEEStatus();							// load status and flags form EEPROM

		while(1)
		{
			switch(GetMode())					// switch to mode according to mode flags	
			{
				case DEBUG_MODE:
					SetMode(FLIGHT_MODE);		// if last mode was DEBUG MODE -> go to FLIGHT MODE
					FlightMode();
					break;

				case FLIGHT_MODE:
					FlightMode();				// go to FLIGHT MODE
					break;

				case POWER_DOWN_MODE:
					PowerDownMode();			// go to POWER DOWN MODE
					break;

				case SAVE_MODE:
					SaveMode();					// go to SAVE MODE
					break;
				
				default:
					SetMode(FLIGHT_MODE);		// default -> go to FLIGHT MODE ??
					FlightMode();
			}

		wdt_reset();							// reset watchdog timer
		}
	}
	return(0);
}

// sets all output pins low and activates pull ups for all inputs
void InitPins(void)
{
	PORTA = (uint8_t) ~((1<<EN_5V_C)|(1<<EN_FET_2_2)|(1<<EN_FET_2_1));
	DDRA = (1<<EN_5V_C)|(1<<EN_FET_2_2)|(1<<EN_FET_2_1);
	PORTB = (uint8_t) ~((1<<BMODE_5V)|(1<<EN_5V_B)|(1<<EN_5V_A)|(1<<EN_5V_D)|(1<<EN_HEATER_1)|(1<<EN_FET_4_2));
	DDRB = (1<<BMODE_5V)|(1<<EN_5V_B)|(1<<EN_5V_A)|(1<<EN_5V_D)|(1<<EN_HEATER_1)|(1<<EN_FET_4_2);
	PORTC = (uint8_t) ~((1<<EN_FET_3_1)|(1<<EN_3V3)|(1<<BMODE_3V3)|(1<<EN_FET_1_1));
	DDRC = (1<<EN_FET_3_1)|(1<<EN_3V3)|(1<<BMODE_3V3)|(1<<EN_FET_1_1);
	PORTD = (uint8_t) ~((1<<EN_FET_5_1)|(1<<EN_FET_5_2)|(1<<EN_5V)|(1<<EN_3V3_A));
	DDRD = (1<<EN_FET_5_1)|(1<<EN_FET_5_2)|(1<<EN_5V)|(1<<EN_3V3_A);
	PORTE = ~(1<<EN_3V3_BACKUP);
	DDRE = 1<<EN_3V3_BACKUP;
	PORTF = ~((1<<EN_3V3_D)|(1<<EN_HEATER_2));
	DDRF = (1<<EN_3V3_D)|(1<<EN_HEATER_2);
	PORTG = ~((1<<EN_3V3_B)|(1<<EN_FET_3_2)|(1<<EN_FET_1_2)|(1<<EN_FET_4_1));
	DDRG = (1<<EN_3V3_B)|(1<<EN_FET_3_2)|(1<<EN_FET_1_2)|(1<<EN_FET_4_1);
}