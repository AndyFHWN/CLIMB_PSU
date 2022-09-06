//---------------------------------------------------------
// Title:		power.c
//
// Author:		Patrick Kappl
//
// Creation:	20.03.2015
//
// Description:	provides functions for the power management
//				mainly checking the voltages and currents
//				around the batteries
//---------------------------------------------------------

#include "power.h"

// global variables
uint16_t vBat1;
uint16_t vBat2;
uint16_t iBat1;
uint16_t iBat2;
uint16_t vPV1;
uint16_t vPV2;
uint16_t tempBat1;
uint16_t tempBat2;

// activates the heaters if one of the batteries is below a threshold temperature
// and disconnects the batteries if they get too hot
// if at least 1 heater is activated the mode changes to power save mode
void CheckBatteryTemperatures(void)
{
	if(tempBat1 < BAT_TEMP_THRESHOLD_L)
	{
		OSR3 |= (1<<OR3_HEATER_1);				// if the temperature of battery 1 is too low
		SetHeater1();							// activate heater 1
		//SetMode(POWER_DOWN_MODE);				// and set mode to power down mode
	}
	if(tempBat2 < BAT_TEMP_THRESHOLD_L)
	{
		OSR3 |= (1<<OR3_HEATER_2);				// if the temperature of battery 2 is too low
		SetHeater2();							// activate heater 2
		//SetMode(POWER_DOWN_MODE);				// and set mode to power down mode
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////
/*	if(tempBat1 > BAT_TEMP_THRESHOLD_U)
	{
		DisconnectBat1();
	}
	else
	{
		ConnectBat1();
	}
	if(tempBat2 > BAT_TEMP_THRESHOLD_U)
	{
		DisconnectBat2();
	}
	else
	{
		ConnectBat2();
	}
*/	///////////////////////////////////////////////////////////////////////////////////////////////
}

// disconnects the battery if the voltage is below or above a threshold
void CheckBatteryVoltages(void)
{
	/*if(abs(vBat1 - vBat2) < BAT_DV_THRESHOLD)
	{
		if(vPV1 > (vBat2 + BAT_V_HYSTERESIS))
		{
			OSR2 |= 1<<OR2_FET_2_1;				// enable FET-2-1 in the OSR
			SetFET21();							// set FET-2-1 according to OVR, FOVR & OSR
		}

		if(vPV2 > (vBat1 + BAT_V_HYSTERESIS))
		{
			OSR2 |= 1<<OR2_FET_1_2;				// enable FET-1-2 in the OSR
			SetFET12();							// set FET-1-2 according to OVR, FOVR & OSR
		}
	}
	
	if(vPV1 > (vBat1 + BAT_V_HYSTERESIS))
	{
		OSR2 |= 1<<OR2_FET_1_1;					// enable FET-1-1 in the OSR
		SetFET11();								// set FET-1-1 according to OVR, FOVR & OSR
	}

	if(vPV2 > (vBat2 + BAT_V_HYSTERESIS))
	{
		OSR2 |= 1<<OR2_FET_2_2;					// enable FET-2-2 in the OSR
		SetFET22();								// set FET-2-2 according to OVR, FOVR & OSR
	}
	
	if((vBat1 < BAT_V_THRESHOLD_L) && (vBat2 < BAT_V_THRESHOLD_L))
	{
		SetMode(POWER_DOWN_MODE);				// change mode to power down mode
	}
	else if(vBat1 < BAT_V_THRESHOLD_L)
	{
		DisconnectBat1();
	}
	else if(vBat2 < BAT_V_THRESHOLD_H)
	{
		DisconnectBat2();
	}
	*/

	// Check Batt1
	if	((vBat1 < BAT_V_THRESHOLD_L) ||
	(vBat1 > BAT_V_THRESHOLD_U) || (tempBat1 > BAT_TEMP_THRESHOLD_U) )
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		DisconnectBat1();
		///////////////////////////////////////////////////////////////////////////////////////////
	}
	if	((vBat1 > BAT_V_THRESHOLD_L + BAT_V_HYSTERESIS) &&
	(vBat1 < BAT_V_THRESHOLD_U - BAT_V_HYSTERESIS) && (tempBat1 < BAT_TEMP_THRESHOLD_U + BAT_TEMP_HYSTERESIS) )
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		ConnectBat1();
		///////////////////////////////////////////////////////////////////////////////////////////
	}
	
	// Check Batt2
	if	((vBat2 < BAT_V_THRESHOLD_L) ||
	(vBat2 > BAT_V_THRESHOLD_U) || (tempBat2 > BAT_TEMP_THRESHOLD_U))
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		DisconnectBat2();
		///////////////////////////////////////////////////////////////////////////////////////////
	}
	if	((vBat2 > BAT_V_THRESHOLD_L + BAT_V_HYSTERESIS) &&
	(vBat2 < BAT_V_THRESHOLD_U - BAT_V_HYSTERESIS) && (tempBat2 < BAT_TEMP_THRESHOLD_U + BAT_TEMP_HYSTERESIS))
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		ConnectBat2();
		///////////////////////////////////////////////////////////////////////////////////////////
	}
}

// sets or resets the temperature and power warning flags if necessary
void CheckBatteryWarnings(void)
{
	if	((tempBat1 < (BAT_TEMP_WARNING_TH + BAT_TEMP_THRESHOLD_L)) ||
		(tempBat2 < (BAT_TEMP_WARNING_TH + BAT_TEMP_THRESHOLD_L)))
	{
		SetTempLowFlag(1);						// set temperature low warning flag
	}
	else
	{
		SetTempLowFlag(0);						// reset power low warning flag
	}
	
	if	((vBat1 < (BAT_V_WARNING_TH + BAT_V_THRESHOLD_L)) ||
		(vBat2 < (BAT_V_WARNING_TH + BAT_V_THRESHOLD_L)))
	{
		SetPowerLowFlag(1);						// set power low warning flag
	}
	else
	{
		SetTempLowFlag(0);						// reset power low warning flag
	}
}

// disconnects the batteries if the current to them is below a certain threshold
void CheckBatteryCurrents(void)
{
	if(iBat1 < BAT_I_THRESHOLD)							// current to battery 1 < threshold?
	{
		OSR2 &= ~((1<<OR2_FET_1_1) | (1<<OR2_FET_1_2));	// disable FET-1-1 and 1-2 in the OSR
		SetFET11();										// set FET-1-1 according to OVR, FOVR & OSR
		SetFET12();										// set FET-1-2 according to OVR, FOVR & OSR
	}

	if(iBat2 < BAT_I_THRESHOLD)							// current to battery 2 < threshold?
	{
		OSR2 &= ~((1<<OR2_FET_2_1) | (1<<OR2_FET_2_2));	// disable FET-2-1 and 2-2 in the OSR
		SetFET11();										// set FET-2-1 according to OVR, FOVR & OSR
		SetFET12();										// set FET-2-2 according to OVR, FOVR & OSR
	}
}

// checks battery current, temperature and voltage or sets power/temperature low flags,
// depending on the bits set in check, the corresponding quantity is checked against thresholds
// CHECK_I, CHECK_T, CHECK_V and CHECK_W are the bit positions for current, temperature, voltage
// and warnings
void CheckBatteries(const uint8_t check)
{
	vBat1 = (global_viBat1.v2High<<8) | (global_viBat1.v2Low);
	vBat2 = (global_viBat2.v2High<<8) | (global_viBat2.v2Low);
	iBat1 = ((global_viBat1.i1High<<8) | (global_viBat1.i1Low)) +
			((global_viBat1.i2High<<8) | (global_viBat1.i2Low));
	iBat2 = ((global_viBat2.i1High<<8) | (global_viBat2.i1Low)) +
			((global_viBat2.i2High<<8) | (global_viBat2.i2Low));
	vPV1 = (global_HV.v1High<<8) | (global_HV.v1Low);				// am bestn mittel wert über olle sensoren ohne ausreißer
	vPV2 = (global_viBat1.v1High<<8) | (global_viBat1.v1Low);
	tempBat1 = (global_tBat1.tempHigh<<8) | (global_tBat1.tempLow);
	tempBat2 = (global_tBat2.tempHigh<<8) | (global_tBat2.tempLow);

	if(((1<<CHECK_I) & check) != 0)
	{
		CheckBatteryCurrents();
	}
	if(((1<<CHECK_T) & check) != 0)
	{
		CheckBatteryTemperatures();
	}
	if(((1<<CHECK_V) & check) != 0)
	{
		CheckBatteryVoltages();
	}
	if(((1<<CHECK_W) & check) != 0)
	{
		CheckBatteryWarnings();
	}
}

// changes the state of the burst mode pin of the switching converters if the current is
// below or above a certain threshold
void CheckBMode(void)
{
	uint16_t i3V3;
	uint16_t i5V;

	i3V3 =	((global_3V3.i1High<<8) | (global_3V3.i1Low)) +
			((global_3V3.i2High<<8) | (global_3V3.i2Low));
	i5V =	((global_5V.i1High<<8) | (global_5V.i1Low)) +
			((global_5V.i2High<<8) | (global_5V.i2Low));

	if(abs(i3V3) > BMODE_I_THRESHOLD)
	{
		OSR3 &= ~(1<<OR3_BMODE_3V3);			// if the current is above a threshold
		SetBMode3V3();							// deactivate the burst mode
	}
	else
	{
		OSR3 |= 1<<OR3_BMODE_3V3;				// else, activate it
		SetBMode3V3();
	}

	if(abs(i5V) > BMODE_I_THRESHOLD)
	{
		OSR3 &= ~(1<<OR3_BMODE_5V);				// if the current is above a threshold
		SetBMode5V();							// deactivate the burst mode
	}
	else
	{
		OSR3 |= 1<<OR3_BMODE_5V;				// else activate it
		SetBMode5V();
	}
}