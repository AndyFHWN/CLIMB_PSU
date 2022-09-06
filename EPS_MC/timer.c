//---------------------------------------------------------
// Title:		timer.c
//
// Author:		Patrick Kappl
//
// Creation:	05.01.2015
//
// Description:	provides functions to use the timers of an
//				ATmega128L the watchdog and the sleep modes
//---------------------------------------------------------

#include "timer.h"

// global variables
uint8_t volatile global_timeout;

// sets and starts timer 1 to cause a timer1 overflow interrupt after the given time in ms
// if the interrupt occurs, the global variable global_timeout is set
// since timer1 is an 16bit timer the max waiting time is 65535ms
// global interrupt enable flag must be set for this to work
void StartTimeout1(uint16_t ms)
{
	TCNT1 = OVF_1MS * ms;					// load timer counter register with the right value
	global_timeout = 0;						// clear timeout
	TIMSK |= 1<<TOIE1;						// enable the timer1 overflow interrupt
	TCCR1B = (1<<CS12)|(1<<CS10);			// start timer1 in normal mode with prescaler 1024
}

// stops timer1 by setting clock select to 0 and disabling the overflow interrupt
void StopTimeout1(void)
{
	TCCR1B = 0x00;							// stop timer1
	TIMSK &= ~(1<<TOIE1);					// disable timer1 overflow interrupt
}

// enters the idle sleep mode for the given time in ms
// max sleep time = 65535ms since timer1 is an 16bit timer
// global interrupt enable flag must be set for this to work
// if ms=0 timer1 overflow interrupt is not enabled and the CPU sleeps indefinitely long
void EnterIdle(uint16_t ms)
{
	global_timeout = 0;						// clear timeout
	if(ms != 0)
	{
		TCNT1 = OVF_1MS * ms;				// load timer counter register with the right value
		TIMSK |= 1<<TOIE1;					// enable the timer1 overflow interrupt
		TCCR1B = (1<<CS12)|(1<<CS10);		// start timer1 in normal mode with prescaler 1024
	}

	set_sleep_mode(SLEEP_MODE_IDLE);		// set sleep mode to idle mode
	sleep_mode();							// go to sleep in the previously set mode
}

// enters the ADC noise reduction mode for the given time in ms
// max sleep time = 255ms since timer0 is an 8-bit timer
// global interrupt enable flag must be set for this to work
// if ms=0 timer0 overflow interrupt is not enabled and the CPU sleeps indefinitely long
void EnterADCNoiseReduction(uint8_t ms)
{
	if(ms != 0)
	{
		TCCR0 = 0x00;							// stop timer0
		TCNT0 = OVF_1MS * ms;					// load timer counter register with the right value
		TIMSK |= 1<<TOIE0;						// enable timer0 overflow interrupt
		TCCR0 = (1<<CS02)|(1<<CS01)|(1<<CS00);	// start timer0 in normal mode with prescaler 1024
	}

	set_sleep_mode(SLEEP_MODE_ADC);				// set sleep mode to ADC noise reduction mode
	sleep_mode();								// go to sleep in the previously set mode
}

// ISR timer 0 overflow interrupt
ISR(TIMER0_OVF_vect)
{
	TCCR0 = 0x00;							// stop timer0
	TIMSK &= ~(1<<TOIE0);					// disable timer0 overflow interrupt
}

// ISR timer 1 overflow interrupt
ISR(TIMER1_OVF_vect)
{
	global_timeout = 1;						// indicate that a timeout has occurred
	StopTimeout1();							// stop timer1
}