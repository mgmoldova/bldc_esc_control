/*
 * uart.c
 *
 * Created: 10.02.2019 20:16:03
 *  Author: master
 */ 
#include "uart.h"

void uart_initialize()
{
	unsigned int baudrate_value=103;
	UBRRL=baudrate_value;                             //Loading the baud rate generator registers
	UBRRH=(baudrate_value>>8);

	UCSRB=(1<<TXEN)|(1<<RXEN);                       //Enabling transmission and reception

	UCSRC=(1<<URSEL)|(3<<UCSZ0);                    //accessing the UCSRC register and selecting 8 bit data format
	write_uart_strg("UART started..\r\n");
}

/*transmit a character through UART*/
void write_uart(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR=data;
	while(!(UCSRA&(1<<TXC)));
}

/*receive a character through UART*/
unsigned char read_uart()
{
	unsigned char data;
	while(!(UCSRA&(1<<RXC)));
	data=UDR;
	return data;
}

/*function transmit a string through UART*/
void write_uart_strg(char *ptr)
{
	while(*ptr)
	{
		write_uart(*ptr);
		ptr++;
	}
}