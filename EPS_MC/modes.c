//---------------------------------------------------------
// Title:		modes.c
//
// Author:		Patrick Kappl
//
// Creation:	04.01.2015
//
// Description:	defines the program sequence in all the
//				different modes in which the µC can be.
//---------------------------------------------------------

#include "modes.h"

#define DEBUG_ 4	// 0='normaler' debug mode, 6=nur battery management
#define CC1 0
#define CC2 1

// used for testing and debugging
void DebugMode(void)
{
	#if DEBUG_ == 8
	//TWSR Register abfragen um aktuellen Wert in der Bitfolge zu bekommen
	
	UARTInit();
	uint8_t twsrcopy;
	char debugstring [100];
	twsrcopy = TWSR;
	sprintf(debugstring,"tws= %d\n",twsrcopy);
	UART1PutS(debugstring, strlen (debugstring));
	
	
	#endif
	
	#if DEBUG_ == 7
	//communication test 1 22.03.2022	
	UARTInit();	
	uint8_t stringa [100];
	while(1)
		{
		sprintf (stringa,"hello\n");
		UART1PutS(*stringa, strlen (stringa));
		while(global_TX1Complete == 0);
		}
		
		/*PORTD |= 1<<PORTD4; //Einschalten des Fet-5-1
		PORTC |= 1<<PORTC1; //Einschalten des 3,3V Converter
		PORTD &= ~1<<PORTD7; //Einschalten des Ausgangs 3V3-A
		//PORTD |= 1<<PORTD7; //Ausschalten des Ausgangs 3V3-A
		
		
		PORTC |= 1<<PORTC0; //Einschalten des Fet-3-1
		PORTD |= 1<<PORTD6; //Einschalten des 5V Converter
		PORTB &= ~1<<PORTB3; //Einschalten des Ausgangs 5V-A
		PORTB |= 1<<PORTB3; //Ausschalten des Ausgangs 5V-A*/
		
	# endif
	
	#if DEBUG_ == 6
		// activate all switching converters and the 12V supply
		EnableAll();
				
		TWIReInit();										// reactivate TWI

		// initialize all VI and temperature sensors
		InitVISensors();
		ConfigTempSensor(&global_tBat1);
		ConfigTempSensor(&global_tBat2);
		ConfigTempSensor(&global_tCenter);
		ConfigTempSensor(&global_tEdge);

		TWIDeInit();
				
		uint8_t dataVector[10];
		UART0ReInit();
		UART1ReInit();
				
		while(1)
		{
			TWIReInit();
			ReadAllSensors();								// read and convert all sensor data
			ConvertAllSensorData();
			TWIDeInit();

			CheckBatteries((1<<CHECK_T) | (1<<CHECK_V));	// check battery voltage and temperature

			sei();
			//uint16_t temp = (global_viBat2.v2High<<8) | (global_viBat2.v2Low);
			dataVector[0] = 0x59;//global_viBat1.v2High;//global_viBat2.v1High;
			dataVector[1] = 0x95;//global_viBat1.v2Low;//global_viBat2.v1Low;
			dataVector[2] = global_viBat2.v2High;
			dataVector[3] = global_viBat2.v2Low;
			dataVector[4] = global_viBat2.i1High;
			dataVector[5] = global_viBat2.i1Low;
			dataVector[6] = global_viBat2.i2High;
			dataVector[7] = global_viBat2.i2Low;
			dataVector[8] = '\n';
					
			UART0PutS(dataVector, 3);
			while(global_TX0Complete == 0);
					
			UART1PutS(dataVector, 1);
			while(global_TX1Complete == 0);
					
			//EnterIdle(400);								// sleep 400ms
			EnterIdle(400);								// sleep 400ms
			cli();

		}
/*		
		// activate all switching converters and the 12V supply
		Switch3V3((1<<A) | (1<<B) | (1<<D) | (1<<BACKUP));
		Switch5V((1<<A) | (1<<B) | (1<<C) | (1<<D));
		SwitchHV(1);
		
		TWIReInit();										// reactivate TWI
		
		// initialize all VI and temperature sensors
		InitVISensors();
		ConfigTempSensor(&global_tBat1);
		ConfigTempSensor(&global_tBat2);
		ConfigTempSensor(&global_tCenter);
		ConfigTempSensor(&global_tEdge);

		TWIDeInit();

		while(1)
		{
			TWIReInit();
			ReadAllSensors();								// read and convert all sensor data
			ConvertAllSensorData();
			TWIDeInit();

			CheckBatteries((1<<CHECK_T) | (1<<CHECK_V));	// check battery voltage and temperature

			sei();
			EnterIdle(400);									// sleep 400ms
			cli();
		}
*/
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 5
		uint8_t dataVector[10];

		sei();
		EnterIdle(2000);
		UART0ReInit();
		TWIReInit();

		InitVISensor(&global_HV);

		while(1)
		{
			ReadVI(&global_HV);
			ConvertVIData(&global_HV);
			dataVector[0] = global_HV.v1High;
			dataVector[1] = global_HV.v1Low;
			dataVector[2] = global_HV.v2High;
			dataVector[3] = global_HV.v2Low;
			dataVector[4] = global_HV.i1High;
			dataVector[5] = global_HV.i1Low;
			dataVector[6] = global_HV.i2High;
			dataVector[7] = global_HV.i2Low;
			
			UART0PutS(dataVector, 8);
			while(global_TX0Complete == 0);

			EnterIdle(1000);
		}

		cli();
		UART0DeInit();
		TWIDeInit();
		while(1);
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 4
		uint8_t dataVector[10];

		sei();
		EnterIdle(2000);
		UART0ReInit();
		TWIReInit();
		
		ConfigTempSensor(&global_tBat2);

		UART0PutS(dataVector, 2);
		while(global_TX0Complete == 0);

		while(1)
		{
			dataVector[0] = TEMPERATURE_REG;
			TWIMasterSendWait(global_tBat2.address, dataVector, 1, 0);
			TWIMasterReceiveWait(global_tBat2.address, dataVector, 2, 1);
			UART0PutS(dataVector, 2);
			while(global_TX0Complete == 0);
			
			ReadTemp(&global_tBat2);
			dataVector[0] = global_tBat2.tempHigh;
			dataVector[1] = global_tBat2.tempLow;
			UART0PutS(dataVector, 2);
			while(global_TX0Complete == 0);

			EnterIdle(1000);
		}

		cli();
		UART0DeInit();
		TWIDeInit();
		while(1);
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 3
		uint8_t data;
		uint8_t dataVector[10];

		sei();
		EnterIdle(3000);
		UART0ReInit();
		TWIReInit();

		dataVector[0] = PRODUCT_ID_REG;
		TWIMasterSendWait(global_3V3.address, dataVector, 1, 0);

		TWIMasterReceiveWait(global_3V3.address, dataVector, 2, 1);
		while((TWCR & 1<<TWSTO) != 0);

		UART0PutS(dataVector, 2);
		while(global_TX0Complete == 0);

		cli();
		UART0DeInit();
		TWIDeInit();
		while(1);
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 2
		uint8_t data;
		uint8_t address = global_tBat2.address;

		sei();
		EnterIdle(4000);
		UART0ReInit();

		TWIReInit();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);

		TWISendStart();
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_START)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendByte((address<<1)|TWIM_WRITE);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MTX_ADR_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendByte(TEMP_CONFIGURATION_REG);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MTX_DATA_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}
		
		data =	(1<<OS)|				// set the sensor to one-shot mode
				(1<<R1)|				// 12bit resolution
				(1<<R0)|
				(1<<POL)|				// alert pin active high
				(1<<TM)|				// interrupt mode and
				(1<<SD);				// enable shutdown mode
		TWISendByte(data);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MTX_DATA_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendStop();
		while((TWCR & 1<<TWSTO) != 0);

		TWISendStart();
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_START)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendByte((address<<1)|TWIM_WRITE);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MTX_ADR_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendByte(TEMPERATURE_REG);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MTX_DATA_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendStart();
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_REP_START)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWISendByte((address<<1)|TWIM_READ);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MRX_ADR_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

		TWIReceiveByte(1);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MRX_DATA_ACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

			data = TWDR;
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);

		TWIReceiveByte(0);
		TWIWaitForComplete();
			data = (TWSR & TWSR_STATUS_MASK);
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);
			if(data != TWI_MRX_DATA_NACK)
			{
				TWISendStop();
				TWIDeInit();
				while(1);
			}

			data = TWDR;
			UART0PutS(&data, 1);
			while(global_TX0Complete == 0);

		TWISendStop();
		TWIDeInit();
		UART0DeInit();
		cli();
		while(1);
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 1
		uint8_t	i = 0;
		uint8_t dataVector[] = "ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ";

		sei();
		EnterIdle(4000);
		while(1)
		{
			UART0ReInit();

			for(i=0;i<100;i++)
			{
				EnterIdle(1);
				UART0PutS(&(dataVector[i]), 1);
				while(global_TX0Complete == 0)
				{
					EnterIdle(0);
				}
			}
			
			while((global_RX0Complete == 0)&&(global_timeout == 0))
			{
				EnterIdle(100);
			}

			UART0DeInit();
			
			dataVector[0] = global_UART0String[0];
			dataVector[1] = global_UART0String[1];
			dataVector[2] = global_UART0String[2];
			dataVector[3] = global_UART0String[3];
			dataVector[4] = global_UART0String[4];
			dataVector[5] = global_UART0String[5];
			dataVector[6] = global_UART0String[6];
			dataVector[7] = global_UART0String[7];
			dataVector[8] = global_UART0String[8];
			dataVector[9] = global_UART0String[9];
			
			global_UART0String[0] = 0;
			global_UART0String[1] = 0;
			global_UART0String[2] = 0;
			global_UART0String[3] = 0;
			global_UART0String[4] = 0;
			global_UART0String[5] = 0;
			global_UART0String[6] = 0;
			global_UART0String[7] = 0;
			global_UART0String[8] = 0;
			global_UART0String[9] = 0;
			
			EnterIdle(100);
		}
	#endif
	//---------------------------------------------------------------------------------------------
	#if DEBUG_ == 0
		uint8_t dataVector[DATA_SEND_LEN];
		uint8_t i;

		DisconnectBat1();						// disconnect the batteries
		DisconnectBat2();
		Switch3V3(1<<A);						// turn on 3V3-A

		while(1)
		{
			#ifdef TEST
				sei();							// set global interrupt enable flag
				test();							// perform the test and never wake up
			#endif
			
			TWIReInit();						// activate TWI
			ReadAllSensors();					// read all VI and temperature sensors
			TWIDeInit();						// deactivate TWI
			ConvertAllSensorData();				// and convert it to the right format and units
			
			CreateDataVector(dataVector);		// create the data vector from the sensor data

			sei();								// set global interrupt flag
			//--- MC to CC2 ---------------------------------------------------------------------------
			#if CC2 == 1
			UART0ReInit();						// activate UART0
			WakeCC2();							// send start byte to wake up CC2
			_delay_ms(PAUSE_MS);				// wait for the duration defined in CCInterface.h
			
/*			for(i=0;i<DATA_SEND_LEN;i++)		// send the data vector to CC2
			{
				//EnterIdle(1);
				UART0PutS(&(dataVector[i]), 1);
				while(global_TX0Complete == 0);
				{
					//EnterIdle(0);
				}
			}
			EnterIdle(10);
*/			
			UART0PutS(dataVector, DATA_SEND_LEN);
			while(global_TX0Complete == 0);
			
			StartTimeout1(500);
			while((global_RX0Complete == 0) && (global_timeout == 0));			
			StopTimeout1();
			
			// set the communication lost flag if a timeout occurred, clear it otherwise
			if(global_timeout == 0) 
			{
				SetCC1CLFlag(0);
			}
			else 
			{
				SetCC1CLFlag(1);
			}
			
			UART0DeInit();						// deactivate UART0
			
			#endif
			//-------------------------------------------------------------------------------------

			//--- MC to CC1 -----------------------------------------------------------------------
			#if CC1 == 1
			UART1ReInit();						// activate UART1
			WakeCC1();							// send start byte to wake up CC1
			_delay_ms(PAUSE_MS);				// wait for the duration defined in CCInterface.h
			
			// send the housekeeping and status data to CC1 and wait till the data has been sent
			UART1PutS(dataVector, DATA_SEND_LEN);
			while(global_TX1Complete == 0);

			// wait for the answer of CC1
			StartTimeout1(500);
			while((global_RX1Complete == 0) && (global_timeout == 0));
			StopTimeout1();
			
			// set the communication lost flag if a timeout occurred, clear it otherwise
			if(global_timeout == 0) SetCC1CLFlag(0);
			else SetCC1CLFlag(1);

			UART1DeInit();						// deactivate UART1
			#endif
			//-------------------------------------------------------------------------------------
			cli();								// clear global interrupt flag
			
			// execute the commands received from CC1 and/or CC2
			// when no time out and CS is okay
			#if CC1 == 1
				if( ( (FlagRegister & (1<<CC1CL))==0 ) && ValidateChecksum(1) ) ExecuteCommands(1);
			#endif
			
			#if CC2 == 1
				if( ( (FlagRegister & (1<<CC2CL))==0 ) && ValidateChecksum(2) ) ExecuteCommands(2);
			#endif
			
			
			sei();
			
			UART1ReInit();
			
			UART1PutS(global_UART0String, DATA_REC_LEN);
			while(global_TX1Complete == 0);		
			
			UART1DeInit();	
			
			for(uint8_t n = 0; n<DATA_REC_LEN; n++)
			{
				global_UART0String[n] = 0;
			}
			
			CheckBatteries((1<<CHECK_T) | (1<<CHECK_V));	// check battery voltage and temperature			
			
			//goto sleep for 0,9s
			sei();
			EnterADCNoiseReduction(200);
			EnterADCNoiseReduction(200);
			EnterADCNoiseReduction(200);
			EnterADCNoiseReduction(200);
			cli();
		}
	#endif
}

// the default mode of the µC:
// all loads can be activated, all sensors are read, communication cycle is normal (~1s)
void FlightMode(void)
{
	uint8_t dataVector[DATA_SEND_LEN];
	uint8_t i;
	uint8_t validAnswerCC1;						// 1 = checksum correct, 0 = checksum wrong
	uint8_t validAnswerCC2;						// 1 = checksum correct, 0 = checksum wrong

	TWIReInit();								// activate TWI
	ReadAllSensors();							// read all VI and temperature sensors
	TWIDeInit();								// deactivate TWI
	ConvertAllSensorData();						// and convert it to the right format and units

	// check battery temperature, various voltages and set warning flags if necessary
	CheckBatteries((1<<CHECK_T) | (1<<CHECK_V) | (1<<CHECK_W));
	CheckBMode();								// check if burst mode should be activated

	CreateDataVector(dataVector);				// create the data vector from the sensor data

	sei();										// set global interrupt flag
	//--- MC to CC2 -------------------------------------------------------------------------------
	#if CC2 == 1
	UART0ReInit();								// activate UART0
	WakeCC2();									// send start byte to wake up CC2
	_delay_ms(PAUSE_MS);						// wait for the duration defined in CCInterface.h

	for(i=0;i<DATA_SEND_LEN;i++)				// send the data vector to CC2
	{
		EnterIdle(1);							// 1ms pause between the bytes to ensure reliable
		UART0PutS(&(dataVector[i]), 1);			// communication
		while(global_TX0Complete == 0)			// wait till the byte has been sent
		{
			EnterIdle(0);
		}
	}

	while((global_RX0Complete == 0) && (global_timeout == 0))
	{
		EnterIdle(UART_TIMEOUT_MS);				// wait for the answer of CC2
	}

	// set the communication lost flag if a timeout occurred, clear it otherwise
	if(global_timeout == 0) SetCC1CLFlag(0);
	else SetCC1CLFlag(1);

	UART0DeInit();								// deactivate UART0
	#endif
	//---------------------------------------------------------------------------------------------

	//--- MC to CC1 -------------------------------------------------------------------------------
	#if CC1 == 1
	UART1ReInit();								// activate UART1
	WakeCC1();									// send start byte to wake up CC1
	_delay_ms(PAUSE_MS);						// wait for the duration defined in CCInterface.h
	
	UART1PutS(dataVector, DATA_SEND_LEN);		// send the housekeeping and status data to CC1
	while(global_TX1Complete == 0)				// wait till the data has been transmitted
	{
		EnterIdle(0);
	}

	while((global_RX1Complete == 0) && (global_timeout == 0))
	{
		EnterIdle(UART_TIMEOUT_MS);				// wait for the answer of CC1
	}

	// set the communication lost flag if a timeout occurred, clear it otherwise
	if(global_timeout == 0) SetCC1CLFlag(0);
	else SetCC1CLFlag(1);

	UART1DeInit();								// deactivate UART1
	#endif
	//---------------------------------------------------------------------------------------------
	cli();										// clear global interrupt flag
	
	if((GetCC1CLFlag() != 0) && (GetCC2CLFlag() != 0))
	{
		SetMode(SAVE_MODE);						// change to save mode if the communication to both
		return;									// CCs is lost
	}

	// check the checksums of the data vectors received from CC1 and CC2
	validAnswerCC1 = CheckChecksum(1);
	validAnswerCC2 = CheckChecksum(2);

	if((validAnswerCC1 == 1) && (validAnswerCC2 == 1))
	{
		if(strcmp((char*) global_UART0String, (char*) global_UART1String) == 0)
		{
			// Execute the transmitted commands if the data vectors match
			ExecuteCommands(1);
		}
		else
		{
			// no error handling implemented yet
		}
	}
	else if(validAnswerCC1 == 1)
	{
		// if only the checksum of CC1 is correct, execute the commands from CC1
		ExecuteCommands(1);
	}
	else
	{
		// if only the checksum of CC2 is correct, execute the commands from CC2
		ExecuteCommands(2);
	}

	UpdateEEPROM();								// update status and flag registers in the EEPROM
	wdt_reset();								// reset the watchdog timer

	//goto sleep for 0,9s
	sei();
	EnterADCNoiseReduction(225);
	EnterADCNoiseReduction(225);
	EnterADCNoiseReduction(225);
	EnterADCNoiseReduction(225);
	cli();

	wdt_reset();								// reset the watchdog timer
}

// if the batteries are low:
// most loads are switched off, less sensors are read, communication cycle is reduced (~10s)
void PowerDownMode(void)
{

}

// if communication errors occur:
// less sensors are read, it's being tried to reconnect, communication cycle is reduced  (~10s)
void SaveMode(void)
{

}