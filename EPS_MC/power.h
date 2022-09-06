//
// power.h
//

#ifndef POWER_H_
	#define POWER_H_ 

	#include "status.h"
	#include "sensors.h"
	#include <stdlib.h>
	
	// temperature in Fix 7.8°C, voltage in UFix 3.13V, current in Fix 2.13A
	#define BAT_TEMP_THRESHOLD_L	0x0000		// lower threshold for battery temperature
	#define BAT_TEMP_THRESHOLD_U	0x4000 //0x2800		// upper threshold for battery temperature
	#define BAT_TEMP_HYSTERESIS 	0x0200		// lower threshold for battery temperature
	#define BAT_TEMP_WARNING_TH		0x0001		// warning threshold for battery temperature
	#define BAT_I_THRESHOLD			0x0010		// threshold for battery current
	#define BAT_V_THRESHOLD_L		0x7000 //0x6000		// lower threshold for battery voltage
	#define BAT_V_THRESHOLD_U		0x8666		// upper threshold for battery voltage
	#define BAT_V_WARNING_TH		0x0001		// warning threshold for battery voltage
	#define BAT_DV_THRESHOLD		0x2000		// threshold for difference in battery voltages
	#define BAT_V_HYSTERESIS		0x0200		// hysteresis of the battery voltage
	#define BMODE_I_THRESHOLD		0x0020		// current threshold for burst mode
	
	// positions in the bitmask of CheckBatteries();
	#define CHECK_I					0
	#define CHECK_T					1
	#define CHECK_V					2
	#define CHECK_W					3

	void CheckBatteryTemperatures(void);
	void CheckBatteryVoltages(void);
	void CheckBatteryWarnings(void);
	void CheckBatteryCurrents(void);
	void CheckBatteries(const uint8_t check);
	void CheckBMode(void);
#endif