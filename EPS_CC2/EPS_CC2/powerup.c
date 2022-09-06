/*
 * powerup.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains the functions used in power-up mode
 */

#include "powerup.h"

void powerup()
{
	io_init();							//initialise I/Os
	timer_init();						//initialise timer
	uart_init();						//initialise UART
	twi_init();							//initialise TWI
	watchdog_init()	;					//initialise watchdog
	set_sleep_mode(SLEEP_MODE_IDLE);	//select sleep mode

	boot_ct();							//increment boot-counter
	status &= ~(1<<MODE_H);				//set mode to flight
	status |= (1<<MODE_L);
	md_ch=0x01;							//set mode changed flag

	data[62]=0xAB;						//ID register

	sei();								//enable interrupts
}

void boot_ct()
{
	//boot counter
	//write value to data[55]
}
