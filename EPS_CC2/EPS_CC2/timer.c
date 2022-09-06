/*
 * timer.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains all around the timer
 */

#include "timer.h"

void timer_init()							//initialise timer
{
	TCCR1B |= 0x03;							//start timer 1 with prescaler 64 -> overflow every 4.19s
	TIMSK |= (1<<TOIE1);					//enable timer 1 overflow interrupt
	main_ct=0;								//reset main counter
	mc_comm_ct=0;							//reset communication timer
}

void watchdog_init()
{
	wdt_enable(WDTO_8S);					//enable watchdog with 8s interval
}

ISR(TIM1_OVF_vect)							//executed every 4.19s
{
	main_ct++;								//main counter
	mc_comm_ct++;							//MC communication timeout counter

	wdt_reset();							//reset watchdog
}

