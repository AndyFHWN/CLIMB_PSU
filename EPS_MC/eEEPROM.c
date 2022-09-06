//---------------------------------------------------------
// Title:		eEEPROM.c
//
// Author:		Patrick Kappl
//
// Creation:	11.02.2015
//
// Description:	provides functions to create, read from and
//				write from ring buffers in the EEPROM in
//				order to enhance the number of total write
//				accesses per variable stored in the EEPROM
//---------------------------------------------------------

#include "eEEPROM.h"

// returns the EEPROM address of the latest value in a ring buffer with the given start address
uint16_t FindCurrentEEPROMAddress(uint16_t bufferStartAddress)
{
	uint8_t oldStatus;
	uint16_t bufferEnd;

	bufferStartAddress += EE_VALUE_BUFFER_SIZE;				// point to the status buffer
	bufferEnd = bufferStartAddress + EE_STATUS_BUFFER_SIZE;	// first address outside the buffer
	
	do	// do as long as the next status is the old status + 1
	{
		oldStatus = EEReadByte(bufferStartAddress);			// save the current status
		bufferStartAddress++;								// point to the next status
		if(bufferStartAddress == bufferEnd)
		{
			break;											// break if we are outside the buffer
		}
	} while (EEReadByte(bufferStartAddress) == oldStatus+1);

	// for the current address subtract 1, for the value address subtract its buffer size
	return (bufferStartAddress - 1 - EE_VALUE_BUFFER_SIZE);
}

// writes to the EEPROM location after(!) the given address (pre-increment)
// if that location would be outside the buffer it writes to bufferStartAddress
void WriteEEPROMBuffer(const uint16_t bufferStartAddress, uint16_t *address, const uint8_t data)
{
	uint8_t oldStatus;

	oldStatus = EEReadByte(*address + EE_VALUE_BUFFER_SIZE);	// save the old status
	
	// if the next location would be outside the buffer, close the loop
	// and go back to the start address -> ring buffer
	(*address)++;
	if(*address == bufferStartAddress + EE_VALUE_BUFFER_SIZE)
	{
		*address = bufferStartAddress;
	}

	// write the data in the value buffer and the incremented status in the status buffer
	// only if the values to be written are different from the ones already in the EEPROM
	EEUpdateByte(address, data);
	EEUpdateByte(address + EE_VALUE_BUFFER_SIZE, oldStatus + 1);
}