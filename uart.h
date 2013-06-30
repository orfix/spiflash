#ifndef  H_UART_MO_1370719196
#define  H_UART_MO_1370719196


#define  NULL	(void*)0

void uart_init(unsigned int ubrr);
void uart_end(void);

void uart_write(char *str);

void uart_putb(unsigned char c);
void uart_putc(unsigned char c);

unsigned char uart_getc(void);
char* uart_gets(char *str, unsigned int size);


#endif  /* Guard */
