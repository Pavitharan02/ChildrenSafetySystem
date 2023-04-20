#include <util/delay.h>
void uart_init()
{

// SETTING THE BIT PARITY
UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	//8 BIT

UCSRB=(1<<RXEN)|(1<<TXEN);// ENABLING TX AND RX

UBRRL=0x4D; // SETTING THE BAUD RATE TO 9600

}



unsigned char rx_data()
{
// WAITING TO RECEIVE THE DATA
while ( !(UCSRA & (1<<RXC)) ); // SETTING THE RX FLAG
UCSRA=(0<<RXC);// CLEAR THE RX FLAG
return UDR;
}

void Tx_String(unsigned char *str)
{
	while(*str)
	{
		tx_data(*str);
		str++;
		_delay_ms(100);
	}
}