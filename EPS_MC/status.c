//---------------------------------------------------------
// Title:		status.c
//
// Author:		Patrick Kappl
//
// Creation:	04.01.2015
//
// Description:	provides all functions that change or read
//				any of the status or flag registers
//---------------------------------------------------------

#include "status.h"

// global variables
uint8_t OSR1;			// output status registers
uint8_t OSR2;			// all output pins are mapped to these registers
uint8_t OSR3;
uint8_t OVR1;			// output value registers
uint8_t OVR2;			// used to overrule the OSRs
uint8_t OVR3;
uint8_t FOVR1;			// force output value registers
uint8_t FOVR2;			// a set bit overrules the OSR with the OVR at that position
uint8_t FOVR3;
uint8_t FlagRegister;

// global variables only for status.c
eeVar_t eeStatus1;		// corresponds to the information in the output registers
eeVar_t eeStatus2;		// but the values are read form PINx
eeVar_t eeStatus3;
eeVar_t eeFlags;		// corresponds to the information in the FlagRegister
eeVar_t eeRebootCount;	// contains the number of reboots without the RBF pin

// gets all the correct addresses used to work with the EEPROM ring buffers
void InitEEPROMRingBuffer(void)
{
	// get the start addresses of the buffers for the different variables
	eeStatus1.bufStartAddr = EE_ADDRESS_VARIABLE_0;
	eeStatus2.bufStartAddr = EE_ADDRESS_VARIABLE_1;
	eeStatus3.bufStartAddr = EE_ADDRESS_VARIABLE_2;
	eeFlags.bufStartAddr = EE_ADDRESS_VARIABLE_3;
	eeRebootCount.bufStartAddr = EE_ADDRESS_VARIABLE_4;

	// get the current addresses of the variables in the different ring buffers
	eeStatus1.curAddr = FindCurrentEEPROMAddress(eeStatus1.bufStartAddr);
	eeStatus2.curAddr = FindCurrentEEPROMAddress(eeStatus2.bufStartAddr);
	eeStatus3.curAddr = FindCurrentEEPROMAddress(eeStatus3.bufStartAddr);
	eeFlags.curAddr = FindCurrentEEPROMAddress(eeFlags.bufStartAddr);
	eeRebootCount.curAddr = FindCurrentEEPROMAddress(eeRebootCount.bufStartAddr);
}

// returns the status byte 1, stored in the EEPROM
uint8_t GetEEStatus1(void)
{
	return(EEReadByte(eeStatus1.curAddr));
}

// returns the status byte 2, stored in the EEPROM
uint8_t GetEEStatus2(void)
{
	return(EEReadByte(eeStatus2.curAddr));
}

// returns the status byte 3, stored in the EEPROM
uint8_t GetEEStatus3(void)
{
	return(EEReadByte(eeStatus3.curAddr));
}

// returns the flags, stored in the EEPROM
uint8_t GetEEFlags(void)
{
	return(EEReadByte(eeFlags.curAddr));
}

// returns the number of reboots without the RBF pin, stored in the EEPROM
uint8_t GetEERebootCount(void)
{
	return(EEReadByte(eeRebootCount.curAddr));
}

// updates the status bytes and flags in the EEPROM with the current data of the µC
void UpdateEEPROM(void)
{
	uint8_t temp;

	// calculate the value of the 1. status byte and save it in the EEPROM
	temp =	(Get3V3A()<<7)|
			(Get3V3B()<<6)|
			(Get3V3D()<<5)|
			(Get3V3Backup()<<4)|
			(Get5VA()<<3)|
			(Get5VB()<<2)|
			(Get5VC()<<1)|
			(Get5VD());
	WriteEEPROMBuffer(eeStatus1.bufStartAddr, &(eeStatus1.curAddr), temp);

	// calculate the value of the 2. status byte and save it in the EEPROM
	temp =	(GetFET11()<<7)|
			(GetFET12()<<6)|
			(GetFET21()<<5)|
			(GetFET22()<<4)|
			(GetFET31()<<3)|
			(GetFET32()<<2)|
			(GetFET41()<<1)|
			(GetFET42());
	WriteEEPROMBuffer(eeStatus2.bufStartAddr, &(eeStatus2.curAddr), temp);

	// calculate the value of the 3. status byte and save it in the EEPROM
	temp =	(GetFET51()<<7)|
			(GetFET52()<<6)|
			(GetHeater1()<<5)|
			(GetHeater2()<<4)|
			(Get3V3()<<3)|
			(GetBMode3V3()<<2)|
			(Get5V()<<1)|
			(GetBMode5V());
	WriteEEPROMBuffer(eeStatus3.bufStartAddr, &(eeStatus3.curAddr), temp);

	// save the FlagRegister in the EEPROM
	WriteEEPROMBuffer(eeFlags.bufStartAddr, &(eeFlags.curAddr), FlagRegister);
}

// saves the new data for the reboot count in the EEPROM
void SetEERebootCount(const uint8_t data)
{
	WriteEEPROMBuffer(eeRebootCount.bufStartAddr, &(eeRebootCount.curAddr), data);
}

// loads the status bytes and the flags from the EEPROM into the OSRs and the FlagRegister
// then sets all outputs accordingly which sets the µC to the state saved in the EEPROM
void LoadEEStatus(void)
{
	OSR1 = GetEEStatus1();				// get the status bytes from the EEPROM
	OSR2 = GetEEStatus2();
	OSR3 = GetEEStatus3();
	FlagRegister = GetEEFlags();		// get the flags from the EEPROM
	SetAllOutputs();					// set all outputs accordingly
}

// clears the CC1CL flag if flag = 0 and sets it otherwise
void SetCC1CLFlag(const uint8_t flag)
{
	if(flag == 0)
	{
		FlagRegister &= ~(1<<CC1CL);				// clear CC1CL flag
	}
	else
	{
		FlagRegister |= 1<<CC1CL;					// set CC1CL flag
	}
}

// returns 0 if the CC1CL flag is cleared and 1 otherwise
uint8_t GetCC1CLFlag(void)
{
	if((FlagRegister & (1<<CC1CL)) == 0)			// if CC1CL is cleared
	{
		return(0);									// return 0
	}
	else
	{
		return(1);									// else return 1
	}
}

// clears the CC2CL flag if flag = 0 and sets it otherwise
void SetCC2CLFlag(const uint8_t flag)
{
	if(flag == 0)
	{
		FlagRegister &= ~(1<<CC2CL);				// clear CC2CL flag
	}
	else
	{
		FlagRegister |= 1<<CC2CL;					// set CC2CL flag
	}
}

// returns 0 if the CC2CL flag is cleared and 1 if it is set
uint8_t GetCC2CLFlag(void)
{
	if((FlagRegister & (1<<CC2CL)) == 0)			// if CC2CL is cleared
	{
		return(0);									// return 0
	}
	else
	{
		return(1);									// else return 1
	}
}

// clears the temp low flag if flag = 0 and sets it otherwise
void SetTempLowFlag(const uint8_t flag)
{
	if(flag == 0)
	{
		FlagRegister &= ~(1<<TEMP_LOW);				// clear temp low flag
	}
	else
	{
		FlagRegister |= 1<<TEMP_LOW;				// set temp low flag
	}
}

// returns 0 if the temp low flag is cleared and 1 if the flag is set
uint8_t GetTempLowFlag(void)
{
	return((FlagRegister & (1<<TEMP_LOW))>>TEMP_LOW);
}

// clears the power low flag if flag = 0 and sets it otherwise
void SetPowerLowFlag(const uint8_t flag)
{
	if(flag == 0)
	{
		FlagRegister &= ~(1<<POWER_LOW);			// clear power low flag
	}
	else
	{
		FlagRegister |= 1<<POWER_LOW;				// set power low flag
	}
}

// returns 0 if the power low flag is cleared and 1 if the flag is set
uint8_t GetPowerLowFlag(void)
{
	return((FlagRegister & (1<<POWER_LOW))>>POWER_LOW);
}

// sets the mode bits in the flag register (bits 2-0) to the given value
void SetMode(const uint8_t modeValue)
{
	FlagRegister = (FlagRegister & ~(MODE_MASK)) | modeValue;	// only change the mode bits
}

// returns the value of the mode bits in the flag register (bits 2-0)
uint8_t GetMode(void)
{
	return(FlagRegister & MODE_MASK);				// only return the mode bits
}

// switches the 3V3 outputs on or off depending on the bit patterns in outputs
// a 1 in outputs switches the output on, a 0 switches it off,
// if all bits are 0 the whole switching converter is turned off
void Switch3V3(const uint8_t outputs)
{
	if((outputs & ~(1<<BACKUP)) == 0)
	{
		// if all bits in outputs are 0, first turn off the different outputs
		OSR1 &= ~((1<<OR1_3V3_A) | (1<<OR1_3V3_B) | (1<<OR1_3V3_D));
		Set3V3A();
		Set3V3B();
		Set3V3D();
		
		// and then turn off the switching converter itself and the FETs
		OSR3 &= ~((1<<OR3_FET_5_1) | (1<<OR3_FET_5_2) | (1<<OR3_3V3));
		Set3V3();
		SetFET51();
		SetFET52();
	}
	else
	{
		// if at least one output should be switched on, enable the FETs and wait
		OSR3 |= (1<<OR3_FET_5_1) | (1<<OR3_FET_5_2);
		SetFET51();
		SetFET52();
		_delay_ms(SC_STARTUP_WAIT);

		// then turn on the switching converter and wait
		OSR3 |= (1<<OR3_3V3);
		Set3V3();
		_delay_ms(SC_STARTUP_WAIT);

		// finally switch the different outputs depending on the bit pattern in outputs
		if((outputs & (1<<A)) == 0) OSR1 &= ~(1<<OR1_3V3_A);
		else OSR1 |= 1<<OR1_3V3_A;
		Set3V3A();

		if((outputs & (1<<B)) == 0) OSR1 &= ~(1<<OR1_3V3_B);
		else OSR1 |= 1<<OR1_3V3_B;
		Set3V3B();

		if((outputs & (1<<D)) == 0) OSR1 &= ~(1<<OR1_3V3_D);
		else OSR1 |= 1<<OR1_3V3_D;
		Set3V3D();
	}
	
	if((outputs & (1<<BACKUP)) == 0) OSR1 &= ~(1<<OR1_3V3_BACKUP);
	else OSR1 |= 1<<OR1_3V3_BACKUP;
	Set3V3Backup();
}

// switches the 5V outputs on or off depending on the bit patterns in outputs
// a 1 in outputs switches the output on, a 0 switches it off,
// if all bits are 0 the whole switching converter is turned off
void Switch5V(const uint8_t outputs)
{
	if(outputs == 0)
	{
		// if all bits in outputs are 0, first turn off the different outputs
		OSR1 &= ~((1<<OR1_5V_A) | (1<<OR1_5V_B) | (1<<OR1_5V_C) | (1<<OR1_5V_D));
		Set5VA();
		Set5VB();
		Set5VC();
		Set5VD();

		// and then turn off the switching converter itself and the FETs
		OSR3 &= ~(1<<OR3_5V);
		OSR2 &= ~((1<<OR2_FET_3_1) | (1<<OR2_FET_3_2));
		Set5V();
		SetFET31();
		SetFET32();
	}
	else
	{
		// if at least one output should be switched on, enable the FETs and wait
		OSR2 |= (1<<OR2_FET_3_1) | (1<<OR2_FET_3_2);
		SetFET31();
		SetFET32();
		_delay_ms(SC_STARTUP_WAIT);

		// then turn on the switching converter and wait
		OSR3 |= (1<<OR3_5V);
		Set5V();
		_delay_ms(SC_STARTUP_WAIT);

		// finally switch the different outputs depending on the bit pattern in outputs
		if((outputs & (1<<A)) == 0) OSR1 &= ~(1<<OR1_5V_A);
		else OSR1 |= 1<<OR1_5V_A;
		Set5VA();

		if((outputs & (1<<B)) == 0) OSR1 &= ~(1<<OR1_5V_B);
		else OSR1 |= 1<<OR1_5V_B;
		Set5VB();

		if((outputs & (1<<C)) == 0) OSR1 &= ~(1<<OR1_5V_C);
		else OSR1 |= 1<<OR1_5V_C;
		Set5VC();

		if((outputs & (1<<D)) == 0) OSR1 &= ~(1<<OR1_5V_D);
		else OSR1 |= 1<<OR1_5V_D;
		Set5VD();
	}
}

// disables FET-4-1 and FET-4-2 if on=0 and enables them otherwise
void SwitchHV(const uint8_t on)
{
	if(on == 0) OSR2 &= ~((1<<OR2_FET_4_1) | (1<<OR2_FET_4_2));
	else OSR2 |= (1<<OR2_FET_4_1) | (1<<OR2_FET_4_2);
	SetFET41();
	SetFET42();
}

// enables FET-1-1 and FET-1-2 thus connecting battery 1 to the PV bus
void ConnectBat1(void)
{
	OSR2 |= (1<<OR2_FET_1_1) | (1<<OR2_FET_1_2);
	SetFET11();
	SetFET12();
}

// disables FET-1-1 and FET-1-2 thus disconnecting battery 1 from the PV bus
void DisconnectBat1(void)
{
	OSR2 &= ~((1<<OR2_FET_1_1) | (1<<OR2_FET_1_2));
	SetFET11();
	SetFET12();
}

// enables FET-2-1 and FET-2-2 thus connecting battery 2 to the PV bus
void ConnectBat2(void)
{
	OSR2 |= (1<<OR2_FET_2_1) | (1<<OR2_FET_2_2);
	SetFET21();
	SetFET22();
}

// disables FET-2-1 and FET-2-2 thus disconnecting battery 2 from the PV bus
void DisconnectBat2(void)
{
	OSR2 &= ~((1<<OR2_FET_2_1) | (1<<OR2_FET_2_2));
	SetFET21();
	SetFET22();
}

// Set: sets or clears the output pin according to OSR, OVR and FOVR
// Get: returns the value of the output pin
// output register 1 ------------------------------------------------------------------------------
void Set3V3A(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_3V3_A)) == 0) PORT_EN_3V3_A &= ~(1<<EN_3V3_A);
	else PORT_EN_3V3_A |= 1<<EN_3V3_A;
}

void Set3V3B(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_3V3_B)) == 0) PORT_EN_3V3_B &= ~(1<<EN_3V3_B);
	else PORT_EN_3V3_B |= 1<<EN_3V3_B;
}

void Set3V3D(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_3V3_D)) == 0) PORT_EN_3V3_D &= ~(1<<EN_3V3_D);
	else PORT_EN_3V3_D |= 1<<EN_3V3_D;
}

void Set3V3Backup(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_3V3_BACKUP)) == 0) PORT_EN_3V3_BACKUP &= ~(1<<EN_3V3_BACKUP);
	else PORT_EN_3V3_BACKUP |= 1<<EN_3V3_BACKUP;
}

void Set5VA(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_5V_A)) == 0) PORT_EN_5V_A &= ~(1<<EN_5V_A);
	else PORT_EN_5V_A |= 1<<EN_5V_A;
}

void Set5VB(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_5V_B)) == 0) PORT_EN_5V_B &= ~(1<<EN_5V_B);
	else PORT_EN_5V_B |= 1<<EN_5V_B;
}

void Set5VC(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_5V_C)) == 0) PORT_EN_5V_C &= ~(1<<EN_5V_C);
	else PORT_EN_5V_C |= 1<<EN_5V_C;
}

void Set5VD(void)
{
	if((((OSR1 & ~(FOVR1))|(OVR1 & FOVR1)) & (1<<OR1_5V_D)) == 0) PORT_EN_5V_D &= ~(1<<EN_5V_D);
	else PORT_EN_5V_D |= 1<<EN_5V_D;
}

uint8_t Get3V3A(void)
{
	return((PIN_EN_3V3_A & (1<<EN_3V3_A))>>EN_3V3_A);
}

uint8_t Get3V3B(void)
{
	return((PIN_EN_3V3_B & (1<<EN_3V3_B))>>EN_3V3_B);
}

uint8_t Get3V3D(void)
{
	return((PIN_EN_3V3_D & (1<<EN_3V3_D))>>EN_3V3_D);
}

uint8_t Get3V3Backup(void)
{
	return((PIN_EN_3V3_BACKUP & (1<<EN_3V3_BACKUP))>>EN_3V3_BACKUP);
}

uint8_t Get5VA(void)
{
	return((PIN_EN_5V_A & (1<<EN_5V_A))>>EN_5V_A);
}

uint8_t Get5VB(void)
{
	return((PIN_EN_5V_B & (1<<EN_5V_B))>>EN_5V_B);
}

uint8_t Get5VC(void)
{
	return((PIN_EN_5V_C & (1<<EN_5V_C))>>EN_5V_C);
}

uint8_t Get5VD(void)
{
	return((PIN_EN_5V_D & (1<<EN_5V_D))>>EN_5V_D);
}

// output register 2 ------------------------------------------------------------------------------
void SetFET11(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_1_1)) == 0) PORT_EN_FET_1_1 &= ~(1<<EN_FET_1_1);
	else PORT_EN_FET_1_1 |= 1<<EN_FET_1_1;
}

void SetFET12(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_1_2)) == 0) PORT_EN_FET_1_2 &= ~(1<<EN_FET_1_2);
	else PORT_EN_FET_1_2 |= 1<<EN_FET_1_2;
}

void SetFET21(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_2_1)) == 0) PORT_EN_FET_2_1 &= ~(1<<EN_FET_2_1);
	else PORT_EN_FET_2_1 |= 1<<EN_FET_2_1;
}

void SetFET22(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_2_2)) == 0) PORT_EN_FET_2_2 &= ~(1<<EN_FET_2_2);
	else PORT_EN_FET_2_2 |= 1<<EN_FET_2_2;
}

void SetFET31(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_3_1)) == 0) PORT_EN_FET_3_1 &= ~(1<<EN_FET_3_1);
	else PORT_EN_FET_3_1 |= 1<<EN_FET_3_1;
}

void SetFET32(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_3_2)) == 0) PORT_EN_FET_3_2 &= ~(1<<EN_FET_3_2);
	else PORT_EN_FET_3_2 |= 1<<EN_FET_3_2;
}

void SetFET41(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_4_1)) == 0) PORT_EN_FET_4_1 &= ~(1<<EN_FET_4_1);
	else PORT_EN_FET_4_1 |= 1<<EN_FET_4_1;
}

void SetFET42(void)
{
	if((((OSR2 & ~(FOVR2))|(OVR2 & FOVR2)) & (1<<OR2_FET_4_2)) == 0) PORT_EN_FET_4_2 &= ~(1<<EN_FET_4_2);
	else PORT_EN_FET_4_2 |= 1<<EN_FET_4_2;
}

uint8_t GetFET11(void)
{
	return((PIN_EN_FET_1_1 & (1<<EN_FET_1_1))>>EN_FET_1_1);
}

uint8_t GetFET12(void)
{
	return((PIN_EN_FET_1_2 & (1<<EN_FET_1_2))>>EN_FET_1_2);
}

uint8_t GetFET21(void)
{
	return((PIN_EN_FET_2_1 & (1<<EN_FET_2_1))>>EN_FET_2_1);
}

uint8_t GetFET22(void)
{
	return((PIN_EN_FET_2_2 & (1<<EN_FET_2_2))>>EN_FET_2_2);
}

uint8_t GetFET31(void)
{
	return((PIN_EN_FET_3_1 & (1<<EN_FET_3_1))>>EN_FET_3_1);
}

uint8_t GetFET32(void)
{
	return((PIN_EN_FET_3_2 & (1<<EN_FET_3_2))>>EN_FET_3_2);
}

uint8_t GetFET41(void)
{
	return((PIN_EN_FET_4_1 & (1<<EN_FET_4_1))>>EN_FET_4_1);
}

uint8_t GetFET42(void)
{
	return((PIN_EN_FET_4_2 & (1<<EN_FET_4_2))>>EN_FET_4_2);
}

// output register 3 ------------------------------------------------------------------------------
void SetFET51(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_FET_5_1)) == 0) PORT_EN_FET_5_1 &= ~(1<<EN_FET_5_1);
	else PORT_EN_FET_5_1 |= 1<<EN_FET_5_1;
}

void SetFET52(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_FET_5_2)) == 0) PORT_EN_FET_5_2 &= ~(1<<EN_FET_5_2);
	else PORT_EN_FET_5_2 |= 1<<EN_FET_5_2;
}

void SetHeater1(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_HEATER_1)) == 0) PORT_EN_HEATER_1 &= ~(1<<EN_HEATER_1);
	else PORT_EN_HEATER_1 |= 1<<EN_HEATER_1;
}

void SetHeater2(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_HEATER_2)) == 0) PORT_EN_HEATER_2 &= ~(1<<EN_HEATER_2);
	else PORT_EN_HEATER_2 |= 1<<EN_HEATER_2;
}

void Set3V3(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_3V3)) == 0) PORT_EN_3V3 &= ~(1<<EN_3V3);
	else PORT_EN_3V3 |= 1<<EN_3V3;
}

void SetBMode3V3(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_BMODE_3V3)) == 0) PORT_BMODE_3V3 &= ~(1<<BMODE_3V3);
	else PORT_BMODE_3V3 |= 1<<BMODE_3V3;
}

void Set5V(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_5V)) == 0) PORT_EN_5V &= ~(1<<EN_5V);
	else PORT_EN_5V |= 1<<EN_5V;
}

void SetBMode5V(void)
{
	if((((OSR3 & ~(FOVR3))|(OVR3 & FOVR3)) & (1<<OR3_BMODE_5V)) == 0) PORT_BMODE_5V &= ~(1<<BMODE_5V);
	else PORT_BMODE_5V |= 1<<BMODE_5V;
}

uint8_t GetFET51(void)
{
	return((PIN_EN_FET_5_1 & (1<<EN_FET_5_1))>>EN_FET_5_1);
}

uint8_t GetFET52(void)
{
	return((PIN_EN_FET_5_2 & (1<<EN_FET_5_2))>>EN_FET_5_2);
}

uint8_t GetHeater1(void)
{
	return((PIN_EN_HEATER_1 & (1<<EN_HEATER_1))>>EN_HEATER_1);
}

uint8_t GetHeater2(void)
{
	return((PIN_EN_HEATER_2 & (1<<EN_HEATER_2))>>EN_HEATER_2);
}

uint8_t Get3V3(void)
{
	return((PIN_EN_3V3 & (1<<EN_3V3))>>EN_3V3);
}

uint8_t GetBMode3V3(void)
{
	return((PIN_BMODE_3V3 & (1<<BMODE_3V3))>>BMODE_3V3);
}

uint8_t Get5V(void)
{
	return((PIN_EN_5V & (1<<EN_5V))>>EN_5V);
}

uint8_t GetBMode5V(void)
{
	return((PIN_BMODE_5V & (1<<BMODE_5V))>>BMODE_5V);
}

// ------------------------------------------------------------------------------------------------

void SetAllOutputs(void)
{
	// output register 1
	Set3V3A();
	Set3V3B();
	Set3V3D();
	Set3V3Backup();
	Set5VA();
	Set5VB();
	Set5VC();
	Set5VD();
	// output register 2
	SetFET11();
	SetFET12();
	SetFET21();
	SetFET22();
	SetFET31();
	SetFET32();
	SetFET41();
	SetFET42();
	// output register 3
	SetFET51();
	SetFET52();
	SetHeater1();
	SetHeater2();
	Set3V3();
	SetBMode3V3();
	Set5V();
	SetBMode5V();
}

// enables all switching converters and the 12V supply
void EnableAll(void)
{
	// activate the FETs of the switching converters and for 12V
	OSR2 |= (1<<OR2_FET_3_1) | (1<<OR2_FET_3_2) | (1<<OR2_FET_4_1) | (1<<OR2_FET_4_2);
	OSR3 |= (1<<OR3_FET_5_1) | (1<<OR3_FET_5_2);
	SetFET31();
	SetFET32();
	SetFET41();
	SetFET42();
	SetFET51();
	SetFET52();
	_delay_ms(SC_STARTUP_WAIT);

	// enable the switching converters
	OSR3 |= (1<<OR3_3V3) | (1<<OR3_5V);
	Set3V3();
	Set5V();
	_delay_ms(SC_STARTUP_WAIT);

	// activate all channels
	OSR1 = 0xFF;
	Set3V3A();
	Set3V3B();
	Set3V3Backup();
	Set3V3D();
	Set5VA();
	Set5VB();
	Set5VC();
	Set5VD();
}