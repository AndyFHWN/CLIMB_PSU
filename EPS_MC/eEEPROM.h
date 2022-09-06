//
// eEEPROM.h
//

#ifndef EEPROM_H_
	#define EEPROM_H_

	#include <avr/eeprom.h>
	#include <inttypes.h>

	// macros to avoid casts when accessing direct EEPROM addresses
	#define EEReadByte(addr)		eeprom_read_byte((uint8_t *)addr)
	#define EEWriteByte(addr, val)	eeprom_write_byte((uint8_t *)addr, val)
	#define EEUpdateByte(addr, val)	eeprom_update_byte((uint8_t *)addr, val)

	// size of the ring buffer in the EEPROM for an 8bit variable
	// the buffer size must not be an integral multiple of 256
	#define EE_VALUE_BUFFER_SIZE	250
	#define EE_STATUS_BUFFER_SIZE	EE_VALUE_BUFFER_SIZE

	// start addresses of the ring buffer for the different EEPROM variables
	#define EE_ADDRESS_VARIABLE_0	0
	#define EE_ADDRESS_VARIABLE_1	(EE_ADDRESS_VARIABLE_0 + EE_VALUE_BUFFER_SIZE + EE_STATUS_BUFFER_SIZE)
	#define EE_ADDRESS_VARIABLE_2	(EE_ADDRESS_VARIABLE_1 + EE_VALUE_BUFFER_SIZE + EE_STATUS_BUFFER_SIZE)
	#define EE_ADDRESS_VARIABLE_3	(EE_ADDRESS_VARIABLE_2 + EE_VALUE_BUFFER_SIZE + EE_STATUS_BUFFER_SIZE)
	#define EE_ADDRESS_VARIABLE_4	(EE_ADDRESS_VARIABLE_3 + EE_VALUE_BUFFER_SIZE + EE_STATUS_BUFFER_SIZE)

	// function prototypes
	uint16_t FindCurrentEEPROMAddress(uint16_t bufferStartAddress);
	void WriteEEPROMBuffer(const uint16_t bufferStartAddress, uint16_t *address, const uint8_t data);
#endif