/*
 * PER ASPERA AD ASTRA!
 * 				Seneca, Hercules furens
 *
 * main.c
 *
 *  Created on: 5 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *
 *      defines all things for main.c
 */

#include "main.h"

int main(void)
{
	powerup();

	while(1)											//main loop
	{	
		//if(~(status & 0x80) && (status & 0x40))			//flight mode?
		//{
			flight();									//flight mode
		//}
		//else
		//{
		//	safe();										//safe mode
		//}
	}
}
