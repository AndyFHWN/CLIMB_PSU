/*
 * EPS_CC2.c
 *
 * Created: 1/24/2015 4:06:05 PM
 *  Author: PATSIE
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <inttypes.h>
	
int main(void)
{
	DDRA |= 0b01000000;
	PORTA &= ~(0b01000000);

    while(1)
    {
        //TODO:: Please write your application code 
    }
}