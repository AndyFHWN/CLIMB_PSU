//
// timer.h
//

#ifndef TIMER_H_
	#define TIMER_H_

	#include <avr/io.h>
	#include <inttypes.h>
	#include <avr/interrupt.h>
	#include <avr/sleep.h>
	#include <avr/wdt.h>

	// with a CPU clock of 1MHz and a prescaler of 1024
	// loading this timer value gives an overflow interrupt after about 1ms (exactly 1.024ms)
	#define OVF_1MS		-1

	// global variables
	extern volatile uint8_t global_timeout;

	// function prototypes
	void StartTimeout1(uint16_t ms);
	void StopTimeout1(void);
	void EnterIdle(uint16_t ms);
	void EnterADCNoiseReduction(uint8_t ms);
#endif