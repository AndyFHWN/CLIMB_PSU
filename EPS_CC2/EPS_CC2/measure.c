/*
 * measure.c
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#include "measure.h"

void measure_vcc()							//measures the supply voltage
{
	uint8_t adc_data;						//ADC result
	float vcc;								//Vcc in Volt


	ADCSRA |= (1<<ADEN);					//enable ADC
	ADMUX &= ~(1<<REFS0);					//set Vcc as reference voltage
	ADMUX |= (1<<REFS1);
	ADMUX |= (1<<MUX0);						//set 1V1 as voltage to be measured
	ADMUX &= ~(1<<MUX1);
	ADMUX |= (1<<MUX2);
	ADMUX |= (1<<MUX3);
	ADCSRB |= (1<<ADLAR);					//result will be 8 bit left adjusted
	_delay_ms(1);							//delay till 1V1 is stable
	ADCSRA |= (1<<ADSC);					//measure
	ADCSRA &= ~(1<<ADEN);					//disable ADC
	adc_data = ADCH;						//read result

	vcc = ADC_REF* 255/adc_data;			//calculate voltage

	if(vcc >= 0 && vcc <= 3.984375)			//result in data range?
	{
		VCC=(uint8_t)vcc*64;				//convert to UFix 3.5 and save
	}

}

void measure_temp()							//measures the internal temperature
{
	uint8_t adc_res;						//ADC result
	float int_temp;							//temperature in Celsius

	ADCSRA |= (1<<ADEN);					//enable ADC
	ADMUX &= ~(1<<REFS0);					//set 1V1 as reference voltage
	ADMUX &= ~(1<<REFS1);
	ADMUX &= ~(1<<MUX0);					//set temperature sensor as channel to be measured
	ADMUX |= (1<<MUX1);
	ADMUX |= (1<<MUX2);
	ADMUX |= (1<<MUX3);
	ADCSRB |= (1<<ADLAR);					//result will be 8 bit left adjusted
	_delay_ms(1);							//delay till 1V1 is stable
	ADCSRA |= (1<<ADSC);					//measure
	ADCSRA &= ~(1<<ADEN);					//disable ADC
	adc_res = ADCH;							//read result

	int_temp = TEMP_C * adc_res + 273.;		//calculate temperature (in Celsius)

	if(int_temp >= -128 && int_temp <= 127)	//result in data range?
	{
		TEMP=(int8_t)int_temp;				//convert to Fix 7.0 and save
	}
}
