//
// ioMC.h
//

#ifndef IOMC_H_
	#define IOMC_H_

	#include <avr/io.h>

	// 3V3
	#define EN_3V3_A			PD7
	#define PORT_EN_3V3_A		PORTD
	#define PIN_EN_3V3_A		PIND
	#define DDR_EN_3V3_A		DDRD

	#define EN_3V3_B			PG0
	#define PORT_EN_3V3_B		PORTG
	#define PIN_EN_3V3_B		PING
	#define DDR_EN_3V3_B		DDRG

	#define EN_3V3_D			PF0
	#define PORT_EN_3V3_D		PORTF
	#define PIN_EN_3V3_D		PINF
	#define DDR_EN_3V3_D		DDRF
	
	#define EN_3V3				PC1
	#define PORT_EN_3V3			PORTC
	#define PIN_EN_3V3			PINC
	#define DDR_EN_3V3			DDRC
	
	// 3V3 burst mode
	#define BMODE_3V3			PC2
	#define PORT_BMODE_3V3		PORTC
	#define PIN_BMODE_3V3		PINC
	#define DDR_BMODE_3V3		DDRC

	// 3V3 backup
	#define EN_3V3_BACKUP		PE2
	#define PORT_EN_3V3_BACKUP	PORTE
	#define PIN_EN_3V3_BACKUP	PINE
	#define DDR_EN_3V3_BACKUP	DDRE

	// 5V
	#define EN_5V_A				PB3
	#define PORT_EN_5V_A		PORTB
	#define PIN_EN_5V_A			PINB
	#define DDR_EN_5V_A			DDRB

	#define EN_5V_B				PB2
	#define PORT_EN_5V_B		PORTB
	#define PIN_EN_5V_B			PINB
	#define DDR_EN_5V_B			DDRB

	#define EN_5V_C				PA2 
	#define PORT_EN_5V_C		PORTA
	#define PIN_EN_5V_C			PINA
	#define DDR_EN_5V_C			DDRA

	#define EN_5V_D				PB4
	#define PORT_EN_5V_D		PORTB
	#define PIN_EN_5V_D			PINB
	#define DDR_EN_5V_D			DDRB

	#define EN_5V				PD6
	#define PORT_EN_5V			PORTD
	#define PIN_EN_5V			PIND
	#define DDR_EN_5V			DDRD

	// 5V burst mode
	#define BMODE_5V			PB0
	#define PORT_BMODE_5V		PORTB
	#define PIN_BMODE_5V		PINB
	#define DDR_BMODE_5V		DDRB

	// FETs
	#define EN_FET_1_1			PC7
	#define PORT_EN_FET_1_1		PORTC
	#define PIN_EN_FET_1_1		PINC
	#define DDR_EN_FET_1_1		DDRC

	#define EN_FET_1_2			PG2
	#define PORT_EN_FET_1_2		PORTG
	#define PIN_EN_FET_1_2		PING
	#define DDR_EN_FET_1_2		DDRG

	#define EN_FET_2_1			PA7
	#define PORT_EN_FET_2_1		PORTA
	#define PIN_EN_FET_2_1		PINA
	#define DDR_EN_FET_2_1		DDRA

	#define EN_FET_2_2			PA6
	#define PORT_EN_FET_2_2		PORTA
	#define PIN_EN_FET_2_2		PINA
	#define DDR_EN_FET_2_2		DDRA

	#define EN_FET_3_1			PC0
	#define PORT_EN_FET_3_1		PORTC
	#define PIN_EN_FET_3_1		PINC
	#define DDR_EN_FET_3_1		DDRC

	#define EN_FET_3_2			PG1
	#define PORT_EN_FET_3_2		PORTG
	#define PIN_EN_FET_3_2		PING
	#define DDR_EN_FET_3_2		DDRG

	#define EN_FET_4_1			PG3
	#define PORT_EN_FET_4_1		PORTG
	#define PIN_EN_FET_4_1		PING
	#define DDR_EN_FET_4_1		DDRG

	#define EN_FET_4_2			PB7
	#define PORT_EN_FET_4_2		PORTB
	#define PIN_EN_FET_4_2		PINB
	#define DDR_EN_FET_4_2		DDRB

	#define EN_FET_5_1			PD4
	#define PORT_EN_FET_5_1		PORTD
	#define PIN_EN_FET_5_1		PIND
	#define DDR_EN_FET_5_1		DDRD

	#define EN_FET_5_2			PD5
	#define PORT_EN_FET_5_2		PORTD
	#define PIN_EN_FET_5_2		PIND
	#define DDR_EN_FET_5_2		DDRD

	// heater
	#define EN_HEATER_1			PB5
	#define PORT_EN_HEATER_1	PORTB
	#define PIN_EN_HEATER_1		PINB
	#define DDR_EN_HEATER_1		DDRB

	#define EN_HEATER_2			PF3
	#define PORT_EN_HEATER_2	PORTF
	#define PIN_EN_HEATER_2		PINF
	#define DDR_EN_HEATER_2		DDRF

	// RBF
	#define RBF_INT				PE3
	#define PORT_RBF_INT		PORTE
	#define PIN_RBF_INT			PINE
	#define DDR_RBF_INT			DDRE
#endif