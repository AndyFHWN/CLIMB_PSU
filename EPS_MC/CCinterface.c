//---------------------------------------------------------
// Title:		CCinterface.c
//
// Author:		Patrick Kappl
//
// Creation:	04.01.2015
//
// Description:	provides functions to enable the communi-
//				cation according to interface control docs
//				between the MC an the CCs
//---------------------------------------------------------

#include "CCinterface.h"

// creates the data vector with all necessary sensor and status information which is
// sent to CC1 and CC2 via UART according to Interface_Control_Document_MC-CC_rev0-1
// (not correctly implemented yet)
 void CreateDataVector(uint8_t *dataVector)
{
	uint8_t i;

	dataVector[0] = '$';
	dataVector[1] = 'D';
	// 5V VI data
	dataVector[2] = 'P'; //global_5V.i1Low;
	dataVector[3] = global_5V.i1High;
	dataVector[4] = global_5V.i2Low;
	dataVector[5] = global_5V.i2High;
	dataVector[6] = global_5V.v1Low;
	dataVector[7] = global_5V.v1High;
	dataVector[8] = global_5V.v2Low;
	dataVector[9] = global_5V.v2High;
	// 3V3 VI data
	dataVector[10] = global_3V3.i1Low;
	dataVector[11] = global_3V3.i1High;
	dataVector[12] = global_3V3.i2Low;
	dataVector[13] = global_3V3.i2High;
	dataVector[14] = global_3V3.v1Low;
	dataVector[15] = global_3V3.v1High;
	dataVector[16] = global_3V3.v2Low;
	dataVector[17] = global_3V3.v2High;
	// edge temperature
	dataVector[18] = global_tEdge.tempLow;
	dataVector[19] = global_tEdge.tempHigh;
	// center temperature
	dataVector[20] = global_tCenter.tempLow;
	dataVector[21] = global_tCenter.tempHigh;
	// HV VI data
	dataVector[22] = global_HV.i1Low;
	dataVector[23] = global_HV.i1High;
	dataVector[24] = global_HV.i2Low;
	dataVector[25] = global_HV.i2High;
	dataVector[26] = global_HV.v1Low;
	dataVector[27] = global_HV.v1High;
	dataVector[28] = global_HV.v2Low;
	dataVector[29] = global_HV.v2High;
	// battery 1 VI data
	dataVector[30] = global_viBat1.i1Low;
	dataVector[31] = global_viBat1.i1High;
	dataVector[32] = global_viBat1.i2Low;
	dataVector[33] = global_viBat1.i2High;
	dataVector[34] = global_viBat1.v1Low;
	dataVector[35] = global_viBat1.v1High;
	dataVector[36] = global_viBat1.v2Low;
	dataVector[37] = global_viBat1.v2High;
	// battery 2 VI data
	dataVector[38] = global_viBat2.i1Low;
	dataVector[39] = global_viBat2.i1High;
	dataVector[40] = global_viBat2.i2Low;
	dataVector[41] = global_viBat2.i2High;
	dataVector[42] = global_viBat2.v1Low;
	dataVector[43] = global_viBat2.v1High;
	dataVector[44] = global_viBat2.v2Low;
	dataVector[45] = global_viBat2.v2High;
	// battery 1 temperature
	dataVector[46] = global_tBat1.tempLow;
	dataVector[47] = global_tBat1.tempHigh;
	// battery 2 temperature
	dataVector[48] = global_tBat2.tempLow;
	dataVector[49] = global_tBat2.tempHigh;
	// status registers (not correctly implemented yet)
	dataVector[50] = GetEEStatus1();
	dataVector[51] = GetEEStatus2();
	dataVector[52] = GetEEStatus3();
	// estimated capacity battery 1 and 2 (not implemented yet)
	dataVector[53] = 0x01;
	dataVector[54] = 0x02;
	// number of reboots MC, CC1 and CC2 (not implemented yet)
	dataVector[55] = GetEERebootCount();
	dataVector[56] = 0x01;
	dataVector[57] = 0x02;
	// VCC_CC1, TEMP_CC1, VCC_CC2, TEMP_CC2 (not implemented yet)
	dataVector[58] = 0x01;
	dataVector[59] = 0x01;
	dataVector[60] = 0x02;
	dataVector[61] = 0x02;
	// status CC1, CC2 (not implemented yet)
	dataVector[62] = 0x01;
	dataVector[63] = 0x02;
	// TBD, TBD, CCx FOVR, CCx OVR (not implemented yet)
	dataVector[64] = 0x33;
	dataVector[65] = 0x33;
	dataVector[66] = 0x33;
	dataVector[67] = 0x33;
	// checksum of byte 0 to 65 via XOR
	dataVector[68] = dataVector[0];
	for(i = 1; i <= CHECKSUM_LIMIT_MC_CC; i++)
	{
		dataVector[68] ^= dataVector[i];
	}
}

// sends the start byte via UART0 to wake up CC1 and enable start of communication
void WakeCC1(void)
{
	uint8_t data = START_BYTE;
	UART1PutS(&data, 1);
	//global_RX1Complete = 0;
}

// sends the start byte via UART1 to wake up CC2 and enable start of communication
void WakeCC2(void)
{
	uint8_t data = START_BYTE;
	UART0PutS(&data, 1);
	//global_RX0Complete = 0;
}

// executes the commands of the data vector from the given CC by setting all outputs accordingly
// but only if the repetition byte = 0, otherwise no new commands were sent
// if CC=1 -> data vector from CC1 is used otherwise the one from CC2 is used
void ExecuteCommands(const uint8_t CC)
{
	uint8_t outputs;
	char *dataVector;

	if(CC == 1)
	{
		dataVector = (char*) global_UART1String;
	}
	else
	{
		dataVector = (char*) global_UART0String;
	}

	if(dataVector[REPETITION_BYTE_NR] == 0)		// check if there are new commands
	{
		// update the FOVRs
		FOVR1 = dataVector[FOVR1_BYTE_NR];
		FOVR2 = dataVector[FOVR2_BYTE_NR];
		FOVR3 = dataVector[FOVR3_BYTE_NR];

		// update the OVRs
		OVR1 = dataVector[OVR1_BYTE_NR];
		OVR2 = dataVector[OVR2_BYTE_NR];
		OVR3 = dataVector[OVR3_BYTE_NR];

		// turn on the HV supply if the current limit is different from 0
		if(dataVector[LIMIT_HV_BYTE_NR] != 0) SwitchHV(1);
		else SwitchHV(0);

		// turn on the 3V3 supply depending on the current limits
		outputs = 0;
		if(dataVector[LIMIT_3V3_A_BYTE_NR] != 0) outputs = 1<<A;
		if(dataVector[LIMIT_3V3_B_BYTE_NR] != 0) outputs |= 1<<B;
		if(dataVector[LIMIT_3V3_D_BYTE_NR] != 0) outputs |= 1<<D;
		if(dataVector[LIMIT_3V3_BACKUP_BYTE_NR] != 0) outputs |= 1<<BACKUP;
		Switch3V3(outputs);

		// turn on the 5V supply depending on the current limits
		outputs = 0;
		if(dataVector[LIMIT_5V_A_BYTE_NR] != 0) outputs = 1<<A;
		if(dataVector[LIMIT_5V_B_BYTE_NR] != 0) outputs |= 1<<B;
		if(dataVector[LIMIT_5V_C_BYTE_NR] != 0) outputs |= 1<<C;
		if(dataVector[LIMIT_5V_D_BYTE_NR] != 0) outputs |= 1<<D;
		Switch5V(outputs);

		// set all outputs except those for 3V3, 5V and 12V supply, to take all potential changes
		// in the FOVRs and OVRs into account
		SetFET11();
		SetFET12();
		SetFET21();
		SetFET22();
		SetHeater1();
		SetHeater2();
		SetBMode3V3();
		SetBMode5V();
	}
}

// returns 1 if the checksums of the data vector received from the given CC is correct, 0 otherwise
// if CC=1 the checksum of CC1 is checked otherwise the checksum of CC2 is checked
uint8_t CheckChecksum(const uint8_t CC)
{
	uint8_t i;
	uint8_t cs;

	if(CC == 1)
	{
		cs = global_UART1String[0];
		for(i = 1; i <= CHECKSUM_LIMIT_CC_MC; i++)
		{
			cs ^= global_UART1String[i];
		}

		if(global_UART1String[CHECKSUM_BYTE_NR] == cs)
		{
			return(1);
		}
		else
		{
			return(0);
		}
	}
	else
	{
		cs = global_UART0String[0];
		for(i = 1; i <= CHECKSUM_LIMIT_CC_MC; i++)
		{
			cs ^= global_UART0String[i];
		}

		if(global_UART0String[CHECKSUM_BYTE_NR] == cs)
		{
			return(1);
		}
		else
		{
			return(0);
		}
	}
}