/*
 * measure.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef MEASURE_H_
#define MEASURE_H_

	//INCLUDES
	#include <util/delay.h>
	#include <avr/io.h>
	#include "main.h"

	//DEFINITIONS
	#define ADC_REF	1.1					//ADC reference voltage
	#define TEMP_C	4.0					//ADC to temperature (Celsius) conversion constant

	//VARIABLES

	//FUNCTION PROTOTYPES
	void measure_vcc();
	void measure_temp();

#endif /* MEASURE_H_ */
