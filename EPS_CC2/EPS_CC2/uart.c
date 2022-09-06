/*
 * uart.c
 *
 *  Created on: 6 Jan 2015
 *      Author: Wolfgang Lubowski
 *
 *      contains the functions for the UART bus to the MC
 */

#include "uart.h"

void uart_init()
{
	UBRR0H = UBRR_VAL >> 8;								//set symbol rate
  	UBRR0L = UBRR_VAL & 0xFF;
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);					//set frame format: 8 data bit, 1 stop bit
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);			//enable receiver, transmitter and interrupt for receive complete

	new_data_flag=0x00;									//clear new data flag
	uart_status=0;
}

void uart_reinit()
{
	UCSR0C = 0x00;										//disable receiver, transmitter and interrupts
	uart_init();										//start UART again
}

ISR(USART0_RXC_vect)
{
	uint8_t i = 0;

	if(uart_status != 1)								//Check UART status
	{													//not in receiving status
/*		if(UDR0==0xAA)									//received byte is wake command?
		{
			//do something or nothing
			uart_status = 3;
		}
*/		if(UDR0=='$')									//received byte is start byte?
		{
			uart_status = 1;							//set UART status to receive
// NA!!!
/*			for(i=0; i<UART_MAXSTRLEN_R; i++)			//clear uart_data
			{
				uart_string_R[i]=0;
			}
*/			uart_data_ct_r=0;							//reset data counter
		}
	}
	else 
	{
		if(uart_data_ct_r == UART_MAXSTRLEN_R-2)		//last byte?
		{
			uart_string_R[uart_data_ct_r]=UDR0;			//read last data
			
			uart_status=2;								//set UART status to saving
			
			mc_comm_ct=0;								//reset MC-timeout
			if(status & 0x20)							//time-out flag set?
			{
				status &= ~(1<<MCT);					//clear MC timeout flag
				status &= ~(1<<MODE_H);					//change to flight mode
				status |= (1<<MODE_L);
				md_ch=1;								//set mode-changed flag
			}
		}
		else
		{
			uart_string_R[uart_data_ct_r]=UDR0;				//read data
			uart_data_ct_r++;								//increment counter			
		}

	}
}

void uart_save_send()
{
	uint8_t cs,i,j;
	//calculate checksum
	cs = 1;
	if(cs==1)											//checksum ok -> save data vector
	{
		j=0;
		for(i=2;i<57;i++)								//save data
		{
			data[j]=uart_string_R[i];
			j++;
		}
		
		data[56]=uart_string_R[58];						//VCC CC1
		data[57]=uart_string_R[59];						//temperature of CC1
		data[60]=uart_string_R[62];						//status CC1
		//data[62]=uart_string_R[64];						//TBD
		data[62]=0xAB;						//ID register
		data[63]=uart_string_R[65];						//TBD
		data[81]=uart_string_R[66];						//CC2 FOVR
		data[82]=uart_string_R[67];						//CC2 OVR
	}

	gen_uart_s_str();									//generate sending string
	uart_data_ct_s=1;									//reset data counter for transmission
	uart_status=3;										//set UART status to sending
	new_data_flag=0x00;									//reset new data flag
	UDR0=uart_string_S[0];								//write first byte to data register
	UCSR0B|=1<<UDRIE0;									//enable interrupt for data register empty
}

void gen_uart_s_str()									//generate the string sent to MC
{
	int i,j;

	uart_string_S[0]='$';								//starts with $A
	uart_string_S[1]='A';

/*	uart_string_S[2]=0x00;	//HV
	uart_string_S[3]=0x00;	//3V3A
	uart_string_S[4]=0x00;	//3V3B
	uart_string_S[5]=0x00;	//3V3D
	uart_string_S[6]=0x01;	//3V3Backup
	uart_string_S[7]=0x00;	//5VA
	uart_string_S[8]=0x00;	//5VB
	uart_string_S[9]=0x00;	//5VC
	uart_string_S[10]=0x00;	//5VD
	uart_string_S[11]=0x00;	//Force Output Register 1
	uart_string_S[12]=0x00;	//Force Output Register 2
	uart_string_S[13]=0x00;	//Force Output Register 3
	uart_string_S[14]=0x00;	//Output Value Register 1
	uart_string_S[15]=0x00;	//Output Value Register 2
	uart_string_S[16]=0x00;	//Output Value Register 3
*/
	j=64;
	for(i=2;i<26;i++)									//registers 64 to 87
	{
		uart_string_S[i]=data[j];
		j++;
	}	
	uart_string_S[3]=0x01;	//3V3A on to supply OBC

	uart_string_S[26]=data[55];							//number of reboots of CC2
	uart_string_S[27]=data[58];							//VCC CC2
	uart_string_S[28]=data[59];							//temperature CC2
	uart_string_S[29]=data[61];							//status CC2	
	
	//calculate checksum
	uart_string_S[30] = 0;
	for (uint8_t n = 0; n <= 29; n++)
	{
		uart_string_S[30] ^= uart_string_S[n];	//checksum
	}
									
}

ISR(USART0_DRE_vect)
{
	if(uart_data_ct_s < UART_MAXSTRLEN_S-1)				//last byte not reached
	{
		UDR0 = uart_string_S[uart_data_ct_s];			//write data to data register
		uart_data_ct_s++;								//increment counter
	}
	else												//last byte reached
	{
		UCSR0B&=~(1<<UDRIE0);							//disable interrupt for empty data register
		uart_status=0;									//set UART status to waiting
		UDR0 = uart_string_S[uart_data_ct_s];			//write last byte to data register
		uart_data_ct_s++;								//increment counter
	}
}
