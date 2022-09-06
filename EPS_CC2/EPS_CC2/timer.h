/*
 * timer.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef TIMER_H_
#define TIMER_H_

	//INCLUDES
	#include <avr/interrupt.h>
	#include <avr/io.h>
	#include <avr/wdt.h>
	#include "main.h"

	//DEFINITIONS

	//VARIABLES
	volatile uint8_t main_ct;					//main counter
	volatile uint8_t mc_comm_ct;				//MC communication timeout counter

	//FUNCTION PROTOTYPES
	void timer_init();
	void watchdog_init();

#endif /* TIMER_H_ */
