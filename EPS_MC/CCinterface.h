//
// CCinterface.h
//

#ifndef CCINTERFACE_H_
	#define CCINTERFACE_H_

	#include "UART.h"
	#include "power.h"

	#define START_BYTE					0xAA	// send this byte first
	#define PAUSE_MS					10		// and wait for this time in ms to wake up CC1/CC2
	#define UART_TIMEOUT_MS				100		// max time between the reception of two bytes via UART
	#define DATA_SEND_LEN				69		// length of the data vector sent to CC1/CC2
	#define CHECKSUM_LIMIT_MC_CC		65		// last byte included in the checksum (MC -> CC)
	#define CHECKSUM_LIMIT_CC_MC		29		// last byte included in the checksum (CC -> MC)

	// position of the different registers in the data vector received form CC1/CC2
	#define LIMIT_HV_BYTE_NR			2		// current limits for the different outputs
	#define LIMIT_3V3_A_BYTE_NR			3
	#define LIMIT_3V3_B_BYTE_NR			4
	#define LIMIT_3V3_D_BYTE_NR			5
	#define LIMIT_3V3_BACKUP_BYTE_NR	6
	#define LIMIT_5V_A_BYTE_NR			7
	#define LIMIT_5V_B_BYTE_NR			8
	#define LIMIT_5V_C_BYTE_NR			9
	#define LIMIT_5V_D_BYTE_NR			10
	#define FOVR1_BYTE_NR				11
	#define FOVR2_BYTE_NR				12
	#define FOVR3_BYTE_NR				13
	#define OVR1_BYTE_NR				14
	#define OVR2_BYTE_NR				15
	#define OVR3_BYTE_NR				16
	#define REPETITION_BYTE_NR			25		// if this byte is 0 the above commands were sent
												// for the first time, else it has been a repetition
	#define CHECKSUM_BYTE_NR			30

	//function prototypes
	void CreateDataVector(uint8_t *dataVector);
	void WakeCC1(void);
	void WakeCC2(void);
	void ExecuteCommands(const uint8_t CC);
	uint8_t CheckChecksum(const uint8_t CC);
#endif