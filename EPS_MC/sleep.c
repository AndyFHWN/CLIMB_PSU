//---------------------------------------------------------
// Title:	sleep.c
//
// Author:	Patrick Kappl
//
// Origin:
//
// Description:	provides functions to enter and exit sleep
//				modes
//
// Log: 04.01.2014 Patrick Kappl
//			created
//---------------------------------------------------------

#include "sleep.h"

// enters the given sleep mode for the given time in ms
// for valid values of sleepMode see sleep.h
void Sleep(uint8_t sleepTimeMs, uint8_t sleepMode)
{
	MCUCR |= (1<<SE)|sleepMode;
}

void DisableSleep(void);