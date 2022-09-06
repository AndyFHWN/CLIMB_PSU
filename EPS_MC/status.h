//
// status.h
//

#ifndef STATUS_H_
	#define STATUS_H_

	#define F_CPU 1000000UL

	#include "ioMC.h"
	#include "eEEPROM.h"
	#include <util/delay.h>

	#define SC_STARTUP_WAIT		10		// delay in ms between enabling the converter
										// and the different channels

	// position of the bits in the (force) output (status/value) registers
	// output register 1
	#define OR1_3V3_A			7
	#define OR1_3V3_B			6
	#define OR1_3V3_D			5
	#define OR1_3V3_BACKUP		4
	#define OR1_5V_A			3
	#define OR1_5V_B			2
	#define OR1_5V_C			1
	#define OR1_5V_D			0
	// output register 2
	#define OR2_FET_1_1			7
	#define OR2_FET_1_2			6
	#define OR2_FET_2_1			5
	#define OR2_FET_2_2			4
	#define OR2_FET_3_1			3
	#define OR2_FET_3_2			2
	#define OR2_FET_4_1			1
	#define OR2_FET_4_2			0
	// output register 3
	#define OR3_FET_5_1			7
	#define OR3_FET_5_2			6
	#define OR3_HEATER_1		5
	#define OR3_HEATER_2		4
	#define OR3_3V3				3
	#define OR3_BMODE_3V3		2
	#define OR3_5V				1
	#define OR3_BMODE_5V		0

	// position of the bits in the flag register
	#define CC1CL				7		// communication controller 1 communication lost
	#define CC2CL				6		// communication controller 2 communication lost
	#define POWER_LOW			5
	#define TEMP_LOW			4
	// bit 3 is reserved and bits 2-0 state the current mode

	// definition of the modes in the flag register and the EE status register
	#define MODE_MASK			0x07	// used to mask out anything but the 3 bits for the mode
	#define DEBUG_MODE			0x00
	#define BOOT_MODE			0x01
	#define FLIGHT_MODE			0x02
	#define POWER_DOWN_MODE		0x03
	#define SAVE_MODE			0x04

	// position of the bits for different outputs of the switching converters
	// used in Enable3V3() and Enable5V()
	#define A					1
	#define B					2
	#define C					3
	#define D					4
	#define BACKUP				C

	// typedefs, structs
	typedef struct eeVar_t 
	{
		uint16_t bufStartAddr;			// start address of the ring buffer in the EEPROM
		uint16_t curAddr;				// current address of the variable in the ring buffer
	}eeVar_t;

	// global variables
	extern uint8_t OSR1;				// output status registers
	extern uint8_t OSR2;				// all output pins are mapped to these registers
	extern uint8_t OSR3;
	extern uint8_t OVR1;				// output value registers
	extern uint8_t OVR2;				// used to overrule the OSRs
	extern uint8_t OVR3;
	extern uint8_t FOVR1;				// force output value registers
	extern uint8_t FOVR2;				// a set bit in the FOVRx overrules the OSRx with the
	extern uint8_t FOVR3;				// bit of the OVRx at that position
	extern uint8_t FlagRegister;
	
	// function prototypes
	void InitEEPROMRingBuffer(void);
	uint8_t GetEEStatus1(void);
	uint8_t GetEEStatus2(void);
	uint8_t GetEEStatus3(void);
	uint8_t GetEEFlags(void);
	uint8_t GetEERebootCount(void);
	void UpdateEEPROM(void);
	void SetEERebootCount(const uint8_t data);
	void LoadEEStatus(void);

	void SetCC1CLFlag(const uint8_t flag);
	uint8_t GetCC1CLFlag(void);
	void SetCC2CLFlag(const uint8_t flag);
	uint8_t GetCC2CLFlag(void);
	void SetTempLowFlag(const uint8_t flag);
	uint8_t GetTempLowFlag(void);
	void SetPowerLowFlag(const uint8_t flag);
	uint8_t GetPowerLowFlag(void);
	void SetMode(const uint8_t modeValue);
	uint8_t GetMode(void);

	void Switch3V3(const uint8_t outputs);
	void Switch5V(const uint8_t outputs);
	void SwitchHV(const uint8_t on);
	
	void ConnectBat1(void);
	void DisconnectBat1(void);
	void ConnectBat2(void);
	void DisconnectBat2(void);

	// output register 1
	void Set3V3A(void);
	void Set3V3B(void);
	void Set3V3D(void);
	void Set3V3Backup(void);
	void Set5VA(void);
	void Set5VB(void);
	void Set5VC(void);
	void Set5VD(void);
	uint8_t Get3V3A(void);
	uint8_t Get3V3B(void);
	uint8_t Get3V3D(void);
	uint8_t Get3V3Backup(void);
	uint8_t Get5VA(void);
	uint8_t Get5VB(void);
	uint8_t Get5VC(void);
	uint8_t Get5VD(void);
	// output register 2
	void SetFET11(void);
	void SetFET12(void);
	void SetFET21(void);
	void SetFET22(void);
	void SetFET31(void);
	void SetFET32(void);
	void SetFET41(void);
	void SetFET42(void);
	uint8_t GetFET11(void);
	uint8_t GetFET12(void);
	uint8_t GetFET21(void);
	uint8_t GetFET22(void);
	uint8_t GetFET31(void);
	uint8_t GetFET32(void);
	uint8_t GetFET41(void);
	uint8_t GetFET42(void);
	// output register 3
	void SetFET51(void);
	void SetFET52(void);
	void SetHeater1(void);
	void SetHeater2(void);
	void Set3V3(void);
	void SetBMode3V3(void);
	void Set5V(void);
	void SetBMode5V(void);
	uint8_t GetFET51(void);
	uint8_t GetFET52(void);
	uint8_t GetHeater1(void);
	uint8_t GetHeater2(void);
	uint8_t Get3V3(void);
	uint8_t GetBMode3V3(void);
	uint8_t Get5V(void);
	uint8_t GetBMode5V(void);

	void SetAllOutputs(void);
	void EnableAll(void);
#endif