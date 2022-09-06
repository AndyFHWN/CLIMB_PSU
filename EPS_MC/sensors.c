  //---------------------------------------------------------
// Title:		sensors.c
//
// Author:		Patrick Kappl
//
// Creation:	04.01.2014
//
// Description:	provides functions to initialize and read
//				the PAC1720 VI and the temperature sensors;
//				also provides global structs containing all
//				relevant information of the sensors
//---------------------------------------------------------

#include "sensors.h"

// global variables
viData_t global_viBat1 = {.address = VI_BAT1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
viData_t global_viBat2 = {.address = VI_BAT2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
viData_t global_3V3 = {.address = VI_3V3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
viData_t global_5V = {.address = VI_5V, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
viData_t global_HV = {.address = VI_5V, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
tempData_t global_tBat1 = {.address = TEMP_BAT1, 0, 0};
tempData_t global_tBat2 = {.address = TEMP_BAT2, 0, 0};
tempData_t global_tCenter = {.address = TEMP_CENTER, 0, 0};
tempData_t global_tEdge = {.address = TEMP_EDGE, 0, 0};

//initializes the PAC1720 sensor with the given address via TWI
void InitVISensor(const viData_t *sensor)
{
	uint8_t data[4];

	data[0] = VI_CONFIGURATION_REG;	// write first data byte to the configuration register
	data[1] = (0<<6)				// disable ALERT Pin for conversions
			| (0<<5)				// disable ALERT Pin for Interrupts
			| (1<<4)				// update Ch2 VSENSE register only after one shot command
			| (1<<3)				// update Ch2 VSOURCE register only after one shot command
			| (0<<2)				// disable Timeout
			| (1<<1)				// update Ch1 VSENSE register only after one shot command
			| (1<<0);				// update Ch1 VSOURCE register only after one shot command

	// next byte is written to subsequent register -> conversion rate register
	data[2] = 0x03;					// 1Hz conversion rate

	// send data in master transmitter mode and send stop
	TWIMasterSendWait(sensor->address, data, 3, 1);

	data[0] = VSOURCE_SAMPLING_REG;	// write first data byte to the VSOURCE sampling register
	data[1] = 0xCC;					// 20ms sample time, no averaging
	// next -> VSENSE Ch1 sampling configuration register 
	data[2] = 0x53;					// 80ms sample time, no averaging, +/-80mV full scale range
	// next -> VSENSE Ch2 sampling configuration register
	data[3] = 0x53;					// 80ms sample time, no averaging, +/-80mV full scale range

	// send data in master transmitter mode and send stop
	TWIMasterSendWait(sensor->address, data, 4, 1);
}

// initializes all PAC1720 sensors via TWI
void InitVISensors(void)
{
	uint8_t data[4];

	data[0] = VI_CONFIGURATION_REG;	// write first data byte to the configuration register
	data[1] = (0<<6)				// disable ALERT Pin for conversions
			| (0<<5)				// disable ALERT Pin for Interrupts
			| (1<<4)				// update Ch2 VSENSE register only after one shot command
			| (1<<3)				// update Ch2 VSOURCE register only after one shot command
			| (0<<2)				// disable Timeout
			| (1<<1)				// update Ch1 VSENSE register only after one shot command
			| (1<<0);				// update Ch1 VSOURCE register only after one shot command

	// next byte is written to subsequent register -> conversion rate register
	data[2] = 0x03;					// 1Hz conversion rate

	// send data to all VI sensors in master transmitter mode and send stop
	TWIMasterSendWait(global_viBat1.address, data, 3, 1);
	TWIMasterSendWait(global_viBat2.address, data, 3, 1);
	TWIMasterSendWait(global_3V3.address, data, 3, 1);
	TWIMasterSendWait(global_5V.address, data, 3, 1);
	TWIMasterSendWait(global_HV.address, data, 3, 1);

	data[0] = VSOURCE_SAMPLING_REG;	// write first data byte to the VSOURCE sampling register
	data[1] = 0xCC;					// 20ms sample time, no averaging
	// next -> VSENSE Ch1 sampling configuration register
	data[2] = 0x53;					// 80ms sample time, no averaging, +/-80mV full scale range
	// next -> VSENSE Ch2 sampling configuration register
	data[3] = 0x53;					// 80ms sample time, no averaging, +/-80mV full scale range

	// send data to all VI sensors in master transmitter mode and send stop
	TWIMasterSendWait(global_viBat1.address, data, 4, 1);
	TWIMasterSendWait(global_viBat2.address, data, 4, 1);
	TWIMasterSendWait(global_3V3.address, data, 4, 1);
	TWIMasterSendWait(global_5V.address, data, 4, 1);
	TWIMasterSendWait(global_HV.address, data, 4, 1);
}

// reads the measured data of the given PAC1720 sensor via TWI
// the read data is stored in sensor.vSNS1High - sensor.vSRC2Low
void ReadVI(viData_t *sensor)
{
	uint8_t data[8];

	// first send the address of the VSENSE Ch1 register in master transmitter mode, no stop
	data[0] = VSENSE_1_REG;
	TWIMasterSendWait(sensor->address, data, 1, 1);

	// then get the 8 byte long data vector in master receiver mode and send stop
	TWIMasterReceiveWait(sensor->address, data, 8, 1);

	// save the data to the correct 16bit variables
	sensor->vSNS1 = (data[0]<<8) | data[1];
	sensor->vSNS2 = (data[2]<<8) | data[3];
	sensor->vSRC1 = (data[4]<<8) | data[5];
	sensor->vSRC2 = (data[6]<<8) | data[7];

	// initiate new measurement by writing an arbitrary value to the one shot register
	data[0] = ONE_SHOT_REG;
	data[1] = 0xFF;
	TWIMasterSendWait(sensor->address, data, 2, 1);	// and send stop
}

// sends configuration data to the given temperature sensor via TWI
void ConfigTempSensor(const tempData_t *sensor)
{
	uint8_t data[2];

	data[0] = TEMP_CONFIGURATION_REG;	// write to configuration register
	data[1] =	(1<<OS)|				// set the sensor to one-shot mode
				(1<<R1)|				// 12bit resolution
				(1<<R0)|
				(1<<POL)|				// alert pin active high
				(1<<TM)|				// interrupt mode and
				(1<<SD);				// enable shutdown mode

	// send configuration data in master transmitter mode and send stop
	TWIMasterSendWait(sensor->address, data, 2, 1);
}

// reads the measured temperature of the sensor with the given address via TWI
// the read data vector is 2 byte long and already in the correct Fix 7.8 format
// [0]: temperature high	[1]: temperature low
void ReadTemp(tempData_t *sensor)
{
	uint8_t data[2];

	data[0] = TEMPERATURE_REG;						// address of the temperature register
	TWIMasterSendWait(sensor->address, data, 1, 0);	// send the address in master transmitter mode
													// do not send stop

	// read 2 bytes in master receiver mode, send stop and save the data to the correct variables
	TWIMasterReceiveWait(sensor->address, data, 2, 1);
	sensor->tempHigh = data[0];
	sensor->tempLow = data[1];
	
	ConfigTempSensor(sensor);						// send configuration data to the sensor
													// this will start a new conversion
}

// reads all VI and temperature sensors
void ReadAllSensors(void)
{
	ReadVI(&global_viBat1);
	ReadVI(&global_viBat2);
	ReadVI(&global_3V3);
	ReadVI(&global_5V);
	ReadVI(&global_HV);
	ReadTemp(&global_tCenter);
	ReadTemp(&global_tEdge);
	ReadTemp(&global_tBat1);
	ReadTemp(&global_tBat2);
}

// converts the received VI data to the correct format and units
// according to Interface_Control_Document_MC-CC_rev0.1
void ConvertVIData(viData_t *sensor)
{
	int16_t current;
	uint16_t voltage;

	// VSENSE is 12 bit left-aligned -> the 4 LSBs are always 0 -> use VSENSE>>4
	// format is Fix 2.13, 2^13 = 8192 -> (...) * 8192
	current = (int16_t) (FSR/R_SENSE * (sensor->vSNS1>>4) / DENOMINATOR_I * 8192.);
	sensor->i1High = current>>8;
	sensor->i1Low = 0xFF & current;

	current = (int16_t) (FSR/R_SENSE * (sensor->vSNS2>>4) / DENOMINATOR_I * 8192.);
	sensor->i2High = current>>8;
	sensor->i2Low = 0xFF & current;

	// VSOURCE is 11 bit left-aligned -> the 5 LSBs are always 0 -> use VSOURCE>>5
	// format is UFix 3.13, 2^13 = 8192 -> (...) * 8192
	voltage = (uint16_t) (40. * (sensor->vSRC1>>5) / (DENOMINATOR_V+1) * 8192.);
	sensor->v1High = voltage>>8;
	sensor->v1Low = 0xFF & voltage;
	
	voltage = (uint16_t) (40. * (sensor->vSRC2>>5) / (DENOMINATOR_V+1) * 8192.);
	sensor->v2High = voltage>>8;
	sensor->v2Low = 0xFF & voltage;
}

// converts all received sensor data to the correct format and units
// according to Interface_Control_Document_MC-CC_rev0.1
void ConvertAllSensorData(void)
{
	ConvertVIData(&global_viBat1);
	ConvertVIData(&global_viBat2);
	ConvertVIData(&global_3V3);
	ConvertVIData(&global_5V);
	ConvertVIData(&global_HV);
}


uint8_t ReadID (uint8_t address)
{
	uint8_t data[8];
		
	// ID reg
	data[0] = 0xFD;
	TWIMasterSendWait(address, data, 1, 1);

	// get reg
	TWIMasterReceiveWait(address, data, 1, 1);
	
	return data[0];
}