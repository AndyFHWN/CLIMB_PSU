/*
 * twi.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef TWI_H_
#define TWI_H_

	//INCLUDES
	#include <avr/interrupt.h>
	#include <avr/io.h>
	#include "main.h"

	//DEFINITIONS
	#define TWI_ADR			0xAA						//TWI address*2 -> 0x55*2=0xAA


	//VARIABLES
	volatile uint8_t twi_status;						//0=waiting, 1=starting, 2=register pointer set, 3=communicating
	volatile uint8_t twi_reg_pt;						//TWI register pointer

	//FUNCTION PROTOTYPES
	void twi_init();


#endif /* TWI_H_ */
