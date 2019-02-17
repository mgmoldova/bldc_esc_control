#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

void init_bldc();
long map(long x, long in_min, long in_max, long out_min, long out_max);
extern int	sprintf(char *__s, const char *__fmt, ...);

void ADC_Init()
{
	DDRA = 0x00;		/* Make ADC port as input */
	ADCSRA = 0x87;		/* Enable ADC, fr/128  */
	ADMUX = 0x40;		/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)
{
	int ADC_value;
	
	ADMUX = (0x40) | (channel & 0x07);/* set input channel to read */
	ADCSRA |= (1<<ADSC);	/* start conversion */
	while((ADCSRA &(1<<ADIF))== 0);	/* monitor end of conversion interrupt flag */
	
	ADCSRA |= (1<<ADIF);	/* clear interrupt flag */
	ADC_value = (int)ADCL;	/* read lower byte */
	ADC_value = ADC_value + (int)ADCH*256;/* read higher 2 bits, Multiply with weightage */

	return ADC_value;		/* return digital value */
}


int i = 3000;
int flag = 0;
char str[256];
char uart[10];
char line[20];
int duty;

int main(void)
{
	uart_initialize();
	UCSRB |= (1 << RXCIE);  //ISR
	sei();
	
	DDRD  = (1 << DDD4); // set output

	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= ( 1<<COM1B1 |1<<COM1B0 |1<<WGM11);		//fast PWM (14) , 
	TCCR1B |= ( 1<<WGM12 |1<<WGM13 |1<<CS11);		//prescale 8
	 

    init_bldc();
	int ADC_Value;
	char str[256];
	ADC_Init();		/* Initialize ADC */
	while(1)
	{
     ADC_Value = ADC_Read(1);/* Read the status on Y-OUT pin using channel 0 */
     sprintf(str, "Y=%d\r\n", ADC_Value); // Y = PA1
	 write_uart_strg(str);
	 duty=map(ADC_Value,0,1023,2000,4000);
	 OCR1B = ICR1 - duty;
	}
		
}	

void init_bldc()
{
	ICR1 = 39999;
	OCR1B = ICR1 - 2000;
	write_uart_strg("3000");
	OCR1B = ICR1 - 3000;
	write_uart_strg("3000");
	_delay_ms(1000);
	OCR1B = ICR1 - 4000;
	write_uart_strg("3500");
	_delay_ms(1000);
	OCR1B = ICR1 - 3000;
	write_uart_strg("4000");
	_delay_ms(1000);
}

ISR(USART_RXC_vect)
{
 	char ReceivedByte;
	ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"
 	UDR = ReceivedByte; // Echo back the received byte back to the computer	
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
