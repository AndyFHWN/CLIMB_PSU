/*
 * flight.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef FLIGHT_H_
	#define FLIGHT_H_

	//INCLUDES
	#include <avr/sleep.h>
	#include "main.h"
	#include "io.h"

	//DEFINITONS
	#define MEASURE_FL	14								//interval for measuring Vcc and temp in flight-mode 14 * 4.19s = 59s
	#define MAX_mc_comm_ct	3							//MC communication timeout after 3 * 4.19s = 12.57s

	//VARIABLES
	extern volatile uint8_t uart_status;
	extern volatile uint8_t main_ct;
	extern volatile uint8_t mc_comm_ct;

	//FUNCTION PROTOTYPES
	void flight();
	void flight_init();


#endif /* FLIGHT_H_ */
