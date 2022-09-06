//
// sensors.h
//

#ifndef SENSORS_H_
	#define SENSORS_H_

	#include "TWI.h"

	// addresses of the registers in the PAC1720
	#define VI_CONFIGURATION_REG	0x00	// general configuration register
	#define CONVERSION_RATE_REG		0x01	// conversion rate register
	#define ONE_SHOT_REG			0x02	// one shot register
	#define VSOURCE_SAMPLING_REG	0x0A	// VSOURCE sampling configuration register
	#define VSENSE_1_SAMPLING_REG	0x0B	// VSENSE Ch1 sampling configuration register
	#define VSENSE_2_SAMPLING_REG	0x0C	// VSENSE Ch2 sampling configuration register
	#define VSENSE_1_REG			0x0D	// Ch1 sense voltage high byte
	#define VSENSE_2_REG			0x0F	// Ch2 sense voltage high byte
	#define VSOURCE_1_REG			0x11	// Ch1 source voltage high byte
	#define VSOURCE_2_REG			0x13	// Ch2 source voltage high byte
	#define PRODUCT_ID_REG			0xFD	// stores a fixed value identifying the sensor

	// addresses of the registers in the temperature sensors
	#define TEMPERATURE_REG			0x00	// temperature register read only!
	#define TEMP_CONFIGURATION_REG	0x01	// general configuration register
	
	// bit positions in the configuration register of the temperature sensors
	#define SD						0		// shutdown mode
	#define TM						1		// thermostat mode
	#define POL						2		// polarity bit
	#define F0						3		// fault queue
	#define F1						4
	#define R0						5		// converter resolution
	#define R1						6
	#define OS						7		// one-shot mode

	// TWI slave addresses of the different sensors
	#define VI_BAT2					0x4A	// VI sensor at battery 2
	#define VI_BAT1					0x4C	// VI sensor at battery 1
	#define VI_3V3					0x4F	// VI sensor at the 3V3 converter
	#define VI_5V					0x28	// VI sensor at the 5V converter
	#define VI_HV					0x2B	// VI sensor at the HV output
	#define TEMP_CENTER				0x4E	// temperature sensor at the center of the EPS
	#define TEMP_EDGE				0x48	// temperature sensor at the edge of the EPS
	#define TEMP_BAT1				0x4D	// temperature sensor at battery 1
	#define TEMP_BAT2				0x49	// temperature sensor at battery 2

	// constants for voltage and current calculations
	#define DENOMINATOR_I			2047.	// since VSENSE sample time is 80ms
	#define DENOMINATOR_V			2047.	// since VSOURCE sample time is 20ms
	#define FSR						80.		// full scale range in mV
	#define R_SENSE					10.		// resistance of R_Sense in mOhm


	// typedefs, structs
	typedef struct viData_t
	{
		// TWI slave address
		uint8_t address;
		
		// data from the PAC1720 sensor
		uint16_t vSRC1;
		uint16_t vSRC2;

		int16_t vSNS1;
		int16_t vSNS2;

		// voltage converted to UFix 3.13V
		uint8_t v1Low;
		uint8_t v1High;
		uint8_t v2Low;
		uint8_t v2High;

		// current converted to Fix 2.13A
		uint8_t i1Low;
		uint8_t i1High;
		uint8_t i2Low;
		uint8_t i2High;
	} viData_t;

	typedef struct tempData_t
	{
		//TWI slave address
		uint8_t address;

		// temperature in Fix 7.8°C format
		uint8_t volatile tempLow;
		uint8_t volatile tempHigh;
	} tempData_t;

	// global variables
	extern viData_t global_viBat1;
	extern viData_t global_viBat2;
	extern viData_t global_3V3;
	extern viData_t global_5V;
	extern viData_t global_HV;
	extern tempData_t global_tBat1;
	extern tempData_t global_tBat2;
	extern tempData_t global_tCenter;
	extern tempData_t global_tEdge;

	// function prototypes
	void InitVISensor(const viData_t *sensor);
	void InitVISensors(void);
	void ReadVI(viData_t *sensor);
	void ConfigTempSensor(const tempData_t *sensor);
	void ReadTemp(tempData_t *sensor);
	void ReadAllSensors(void);
	uint8_t ReadID (uint8_t address);
	
	void ConvertVIData(viData_t *sensor);
	void ConvertAllSensorData(void);
#endif