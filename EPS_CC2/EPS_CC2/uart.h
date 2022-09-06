/*
 * uart.h
 *
 *  Created on: 9 Feb 2015
 *      Author: Wolfgang Lubowski
 */

#ifndef UART_H_
#define UART_H_

	//INCLUDES
	#include <avr/interrupt.h>
	#include <avr/io.h>
	#include <util/delay.h>
	#include "main.h"
	#include "twi.h"


	//DEFINITIONS
	#define BAUD 10400UL										//symbol rate of interface: 10.4 kBd
	#define UART_MAXSTRLEN_R 69									//length of vector receiving from MC
	#define UART_MAXSTRLEN_S 31									//length of vector sending to MC
	//calculations
	#define UBRR_VAL (((F_CPU)/(BAUD*16))-1)					//round clever
	#define BAUD_REAL ((F_CPU)/(16*(UBRR_VAL+1)))				//real symbol rate in Bd
	#define BAUD_ERROR (((BAUD_REAL)*1000)/(BAUD))				//error in per mil, 1000 = no error
	#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))					//check error
		#error Baud rate error greater 1% which is too high!
	#endif


	//VARIABLES
	volatile char uart_string_R[UART_MAXSTRLEN_R];				//data vector from MC to OBC, received on UART
	volatile char uart_string_S[UART_MAXSTRLEN_S];				//data vector form OBC to MC, to be sent on UART
	volatile uint8_t uart_status;								//UART status: 0 waiting, 1 receiving, 2 saving, 3 sending
	volatile uint8_t uart_data_ct_r;							//UART data counter for reception
	volatile uint8_t uart_data_ct_s;							//UART data counter for transmission

	extern volatile uint8_t mc_comm_ct;


	//FUNCTION PROTOTYPES
	void uart_init();											//initialises UART
	void uart_save_send();										//calculates checksum, save received string and start sending
	void gen_uart_s_str();										//generates the vector to be sent on UART to MC
	void uart_reinit();											//restart UART in safe mode (might solve the problem)

#endif /* UART_H_ */
