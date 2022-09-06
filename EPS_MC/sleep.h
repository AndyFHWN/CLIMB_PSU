//
// sleep.h
//

#ifndef SLEEP_H_
	#define SLEEP_H_
	
	// sleep modes for use in Sleep()
	#define SM_IDLE			0x00
	#define SM_POWER_DOWN	0x10
	#define SM_POWER_SAVE	0x18

	// function prototypes
	void Sleep(uint8_t sleepTimeMs, uint8_t sleepMode);
	void DisableSleep(void);
#endif