#include  <avr/io.h>
#include  "uart.h"

#define  DUMMY_CMD	0x00
#define  RDID_CMD	0x9F
#define  READ_CMD	0x03
#define  SENDTOFLASH(cmd) 	do{							\
							SPDR = (cmd);				\
							while(!(SPSR & (1<<SPIF)));	\
						}while(0)

int main( void )
{
	uint8_t sector;
	uint8_t page;
	uint8_t byte;

	uart_init(8); /* 115200 baud */

	DDRB = _BV(PB3) | _BV(PB5) | _BV(PB2); 	/* set MOSI, SCK and SS to OUTPUT */
	SPCR = _BV(SPE) | _BV(MSTR); 			/* Mode 0, MSB first, enable SPI, Master mode */
	SPSR = _BV(SPI2X); 						/* double clock speed ...if supported... */

	while(1) {
		uart_getc();
		PORTB &= ~(_BV(PB2)); /* select */

		SENDTOFLASH(READ_CMD);
		/* send address */
		SENDTOFLASH(0x00);
		SENDTOFLASH(0x00);
		SENDTOFLASH(0x00);
		/* 2,097,152 bytes == 8192 pages == 32 sector */
		for( sector=0, page=0, byte=0; sector<32; sector++) {
			do { 
				do {
					SENDTOFLASH(DUMMY_CMD);
					uart_putc(SPDR);
				}while(byte++ < 255);
			}while(page++ < 255);
		}
		PORTB |= _BV(PB2); /* unselect */
	}

	return 0;
}
