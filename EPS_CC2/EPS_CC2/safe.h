/*
 * safe.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef SAFE_H_
#define SAFE_H_

	//INCLUDES
	#include <avr/sleep.h>
	#include "main.h"

	//DEFINITIONS
	#define WAKE_SAFE	3						//interval to wake in safe-mode 3*4.19s=12.57s
	#define T_TH	-20							//minimum temperature for battery1
	#define T_TH_HYS 2
	#define V_TH	2.5							//minimum voltage for battery1
	#define V_TH_HYS 0.1

	//VARIABLES
	extern volatile uint8_t main_ct;			//main counter

	//FUNCTION PROTOTYPES
	void safe();								//safe mode
	void safe_init();							//initialise safe-mode


#endif /* SAFE_H_ */
