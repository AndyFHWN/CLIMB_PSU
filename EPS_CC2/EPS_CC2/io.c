/*
 * io.c
 *
 *  Created on: 10 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#include "io.h"

void io_init()
{
	uint8_t i;
	for(i=0;i<8;i++)					//clear force output value register
	{
		FOVR=0x00;
	}

	PUEA = 0xBC;						//activate pull-ups for bus and all unused pins
	PORTA = 0x00;						//clear PORTA
	status |= (1<<I2C);					//enable I2C
	status &= ~(1<<BAT1);				//disable battery 1
	status &= ~(1<<BACK);				//disable 3v3 backup
	set_outputs();						//execute output settings
	DDRA = (1<<PA0)|(1<<PA1)|(1<<PA6); 	//define PA0,PA1 and PA6 as outputs

	PUEB = 0x0F;						//activate pull-ups for bus and all unused pins, all pins on port B are inputs

	PUEC = 0x3F;						//activate pull-ups for bus and all unused pins, all pins on port C are inputs
}

void set_outputs()						//sets the output pins according to FOVR, OVR and status of CC2
{
	//EN I2C
	/*if(FOVR & 0x01)
	{
		if(OVR & 0x01)
		{
			PORTA |= (1<<PA0);
		}
		else
		{
			PORTA &= ~(1<<PA0);
		}
	}
	else
	{
		if(status & 0x08)
		{
			PORTA |= (1<<PA0);
		}
		else
		{
			PORTA &= ~(1<<PA0);
		}
	}*/
	PORTA |= (1<<PA0);


	//BATTERY 1
	if(FOVR & 0x02)
		{
			if(OVR & 0x02)
			{
				PORTA |= (1<<PA1);
			}
			else
			{
				PORTA &= ~(1<<PA1);
			}
		}
		else
		{
			if(status & 0x04)
			{
				PORTA |= (1<<PA1);
			}
			else
			{
				PORTA &= ~(1<<PA1);
			}
		}

	//3V3 BACKUP
	if(FOVR & 0x04)
			{
				if(OVR & 0x04)
				{
					PORTA |= (1<<PA6);
				}
				else
				{
					PORTA &= ~(1<<PA6);
				}
			}
			else
			{
				if(status & 0x01)
				{
					PORTA |= (1<<PA6);
				}
				else
				{
					PORTA &= ~(1<<PA6);
				}
			}
}
