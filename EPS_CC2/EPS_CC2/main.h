/*
 * main.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef MAIN_H_
#define MAIN_H_

	//INCLUDES
	#include <avr/interrupt.h>

	//DEFINITIONS
	#define TWI_REG_NUM		88					//number of registers used in communication

	#define rb_ct				data[55]		//reboot counter of
	#define VCC					data[58]		//supply voltage Vcc
	#define TEMP				data[59]		//internal temperature
	#define status				data[61]		//status of CC2
	#define FOVR				data[81]		//force output value register
	#define OVR					data[82]		//output value register
	#define new_data_flag		data[87]		//if new data from OBC received since last com. with MC 1, else 0

	//for status byte
	#define MODE_H	8							//mode high and low
	#define MODE_L	6
	#define MCT		5							//MC timeout flag
	#define I2C		3							//enable I2C2
	#define BAT1	2							//enable Fet-1-1-2 to connect BAT1
	#define BACK	0							//enable 3V3 Backup


	//VARIABLES
	volatile char data[TWI_REG_NUM];			//data array for TWI
	volatile uint8_t md_ch;						//mode changed flag

	extern volatile uint8_t uart_status;		//UART status: 0 waiting, 1 receiving, 2 saving, 3 sending

#endif /* MAIN_H_ */
