#include  <avr/io.h>
#include  "uart.h"

/* TODO write some defines to make things clearer ! */
/* TODO write a more flexible uart_init function */

void uart_init(unsigned int ubrr)
{
	/* 16 Mhz
	 * Bit Rate	UBRR	% of error
	 * ---------------------------
	 * 300		3332	0.0
	 * 600		1666	0.0
	 * 1200		832		0.0
	 * 2400		416		0.1
	 * 4800		207		0.2
	 * 9600		103		0.2
	 * 14400	68		0.6
	 * 19200	51		0.2
	 * 28800	34		0.8
	 * 38400	25		0.2
	 * 57600	16		2.1
	 * 76800	12		0.2
	 * 115200	8		3.7
	 * 230400	3		7.8
	 */
	UBRR0 = ubrr; /* DF_CPU/(baud*16) -1 */

	/* Enable TX and RX */
	UCSR0B |= 1<<RXEN0 | 1<<TXEN0;

	/* -- The frame format is by default set to 8N1 */
}

void uart_end(void) 
{
	/* Disable TX and RX */
	UCSR0B &= ~(1<<RXEN0 | 1<<TXEN0);
}

void uart_write(char *str) 
{
	while(*str!='\0') {
		while(! (UCSR0A & 1<<UDRE0)); /* wait until UDR0 is ready */
		UDR0 = *str; /* push a character in the transmit buffer */
		str++;
	}
}

void uart_putb(unsigned char c)
{
	unsigned char i = 7;

	do {
		while(! (UCSR0A & 1<<UDRE0));
		UDR0 = !!(c&(1<<i)) ? '1' : '0';
	}while(i--);
}

void uart_putc(unsigned char c)
{
	while(! (UCSR0A & 1<<UDRE0));
	UDR0 = c;
}

unsigned char uart_getc(void)
{
	while(! (UCSR0A & 1<<RXC0));	

	return UDR0;
}


char* uart_gets(char *str, unsigned int size) 
{
	/* TODO check for errors: parity, overrun, and return NULL */
	/* CR+LF or LF */
	char *p = str;
	if(str!=NULL && size>2) {
		do {
			while(! (UCSR0A & 1<<RXC0));	
			*p = UDR0;

			if(*p=='\r') {
				break;
			} 
			/* if(*p == '\n') {
				break;
			}*/
			p++;
		}while(--size > 1);
		*p = '\0';

		return str;
	}
	return NULL;
}

