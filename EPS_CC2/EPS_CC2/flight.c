/*
 * flight.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains the functions used in flight mode
 */

#include "flight.h"

void flight()
{
	if(md_ch)									//just entered flight mode?
	{
		flight_init();
	}

	if(uart_status==2)							//UART to be answered?
	{
		uart_save_send();
	}

	if(main_ct >= MEASURE_FL)					//time for measuring Vcc and temp?
	{
		measure_vcc();							//measure Vcc and temp
		measure_temp();

		main_ct=0;								//reset main counter
	}

	if(mc_comm_ct >= MAX_mc_comm_ct)			//MC communication timeout?
	{
		status |= (1<<MCT);						//set MC timeout flag
		status |= (1<<MODE_H);					//change to safe mode
		status &= ~(1<<MODE_L);
		md_ch=1;								//set mode-changed flag
	} else {
		sleep_enable();							//go to sleep
		sleep_cpu();
		sleep_disable();						//wake up
	}
}

void flight_init()
{
	status &= ~(1<<BAT1);						//disable battery 1
	status &= ~(1<<BACK);						//disable 3v3 backup
	set_outputs();								//execute output settings

	md_ch=0;									//clear mode-changed flag
}
