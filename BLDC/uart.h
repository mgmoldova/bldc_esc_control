/*
 * uart.h
 *
 * Created: 10.02.2019 20:19:39
 *  Author: master
 */ 


#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
void uart_initialize();
void write_uart(unsigned char data);
unsigned char read_uart();
void write_uart_strg(char *ptr);

#endif /* UART_H_ */