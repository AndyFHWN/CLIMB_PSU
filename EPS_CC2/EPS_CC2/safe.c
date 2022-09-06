/*
 * safemode.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains the functions used in safe-mode
 */

#include "safe.h"

void safe()
{
	if(md_ch)											//just entered safe mode?
	{
		safe_init();
	}

	if(main_ct >= WAKE_SAFE)							//time for measuring Vcc and temp?
	{
		measure_vcc();									//measure Vcc and temp
		measure_temp();

		if(TEMP < T_TH || VCC < V_TH)					//too cold or to low voltage for battery1?
		{
			status &= ~(1<<BAT1);						//disconnect battery1
		}
		if(TEMP>T_TH+T_TH_HYS && VCC>V_TH+V_TH_HYS)		//again warm enough and enough voltage?
		{
			status |= (1<<BAT1);						//connect battery1
		}
		set_outputs();									//execute changes

		uart_reinit();									//restart UART

		main_ct=0;										//reset main counter
	}
	sleep_enable();										//go to sleep
	sleep_cpu();
	sleep_disable();									//wake up
}

void safe_init()
{
	status |= (1<<BAT1);								//connect battery 1
	status |= (1<<BACK);								//connect 3v3 backup
	set_outputs();										//execute output settings

	md_ch=0;											//clear mode-changed flag
}


