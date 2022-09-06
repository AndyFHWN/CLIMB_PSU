//
// modes.h
//

#ifndef MODES_H_
	#define MODES_H_

	#include "CCinterface.h"
	#include "timer.h"
	#include "test.h"
	#include <string.h>
	//extern uint8_t volatile global_RX0Complete;
	
	// function prototypes
	void DebugMode(void);
	void FlightMode(void);
	void PowerDownMode(void);
	void SaveMode(void);
#endif