

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void USART_init(long USART_BAUDRATE)
{
	UCSRB |= (1<<RXCIE) | (1 << RXEN) | (1 << TXEN);    /* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); /* Use 8-bit character sizes */

	UBRRL = BAUD_PRESCALE; 					/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);			/*Load upper 8-bits*/
}

unsigned char USART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0); /*Do nothing until data have been received*/
	return(UDR);				/* return the byte*/
}

void USART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE))); /*Wait for empty transmit buffer*/
	UDR = ch ;
}

void USART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)	/*send string up to null */
	{
		USART_TxChar(str[j]);	
		j++;
	}
}

