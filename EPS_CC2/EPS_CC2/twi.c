/*
 * tiny_twi.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains the functions for the TWI bus to the OBC
 */

#include "twi.h"

void twi_init()
{
	int i;

	TWSA=TWI_ADR;														//set TWI address (caveat: *2)
	TWSCRA = 0x3D;														//enable TWI module, data and address/stop interrupt and smart mode
	twi_status=0;														//set TWI status as waiting
	new_data_flag=0x00;													//reset new data flag

	for(i=0;i<TWI_REG_NUM;i++)
	{
		data[i]=0;
	}
}

ISR(TWI_vect)															//does the TWI communication
{
	if((TWSSRA & 0x40)&&(TWSSRA & 0x01)&&(!(TWSSRA & 0x0C)))			//address interrupt, no collisions or bus errors
	{
		if(twi_status==0)												//TWI is waiting
		{
			twi_status=1;												//set status to starting
			TWSCRB |= 0x03;												//send acknowledge
		}
		else if(twi_status==2)											//status is register pointer set
		{
			TWSCRB |= 0x03;												//send acknowledge, status is unchanged
		}
		else
		{
			twi_status=0;												//unexpected case, reset TWI
			TWSSRA |= 1<<TWASIF;
			TWSSRA |= 1<<TWDIF;
		}
	}
	else if((TWSSRA & 0x80)&&(!(TWSSRA & 0x0C)))						//data interrupt, no collisions or bus errors
	{
		if(twi_status==1)												//status is starting
		{
			twi_reg_pt=TWSD;											//set register pointer
			twi_status=2;												//set status to register pointer set
		}
		else if(twi_status==2 || twi_status==3)							//status is register pointer set or communicating
		{
			twi_status=3;												//set status to communicating
			if(twi_reg_pt<TWI_REG_NUM)									//valid register pointer
			{
				if(TWSSRA & 0x02)										//sending data
				{
					TWSD=data[twi_reg_pt];								//write to data TWI register
					twi_reg_pt++;										//increment pointer
				}
				else													//receiving data
				{
					data[twi_reg_pt]=TWSD;								//read data from TWI register
					twi_reg_pt++;										//increment pointer
					new_data_flag=0x01;									//set new data flag
				}
			}
			else														//invalid register pointer
			{
				twi_status=0;											//unexpected case, reset TWI
				TWSSRA |= 1<<TWASIF;
				TWSSRA |= 1<<TWDIF;
			}
		}
	}
	else if((TWSSRA & 0x40)&&(!(TWSSRA & 0x01))&&(!(TWSSRA & 0x0C)))	//stop interrupt, no collisions or bus errors
	{
		if(twi_status!=2)
		{
			twi_status=0;												//communication, so reset TWI
		}
		TWSSRA &= ~(1<<TWAS);											//clear stop-bit
	}
	else
	{
		twi_status=0;													//unexpected case, reset TWI
		TWSSRA |= 1<<TWASIF;
		TWSSRA |= 1<<TWDIF;
	}
}
