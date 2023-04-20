#define F_CPU 8000000UL		/* define Clock Frequency */
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SREG   _SFR_IO8(0x3F)
#include<avr/io.h> // AVR header
#include<util/delay.h> // delay header
#include "LCD.h"	/* include LCD Header file */
#define LCD_DATA PORTB // port B is selected as LCD data port
#define ctrl PORTD // port D is selected as LCD command port
#define en PD7 // enable signal is connected to port D pin 7
#define rw PD6 // read/write signal is connected to port D pin 6
#define rs PD3 // register select signal is connected to port D pin 5


void uart_init()
{
	// SETTING THE BIT PARITY
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	//8 BIT
	UCSRB=(1<<RXEN)|(1<<TXEN);// ENABLING TX AND RX
	UBRRL = (F_CPU / 16 / 9600) - 1; // SETTING THE BAUD RATE TO 9600
}


unsigned char rx_data()
{
	// WAITING TO RECEIVE THE DATA
	while ( !(UCSRA & (1<<RXC)) ); // SETTING THE RX FLAG
	UCSRA=(0<<RXC);	// CLEAR THE RX FLAG
	return UDR;
}




int main()
{	
	while(1)
	{
		lcd();
		rfid();
	}
	
	return 0;
}

void lcd()
{  // int k="";
	DDRB=0xFF; // set LCD data port as output
	DDRD=0xC8; // set LCD signals (RS, RW, E) as out put
	init_LCD(); // initialize LCD
	_delay_ms(10); // delay of 100 Milli seconds
	LCD_cmd(0x0C); // display on, cursor off
	_delay_ms(10);
	
	LCD_Write_String("Show Your");
	_delay_ms(50);
	LCD_cmd(0xC0); // move cursor to the start of 2nd line
	_delay_ms(100);
	LCD_Write_String("RFID Card");
}

void rfid()
{
	DDRA = DDRA | (1<<6);
	DDRA = DDRA | (1<<7);	
	// setting pins as output for demultiplexer selection pins
	
	PORTA = PORTA & (~(1<<6));
	PORTA = PORTA & (~(1<<7));	
	// setting demultiplexer for RFID
	
	unsigned char s[4],id[12];
	unsigned char a[4][12]={{"13006F6B5391"},{"13006FA337F5"},{"13006F4B6773"},{"13006FC351A2"}};
	unsigned char name[4][10]={{"Henry"},{"David"},{"James"},{"Peter"}};
	// assigning RFID numbers for 4 students

	uart_init(); // RFID starting to function
	_delay_ms(100);
	
		for(int j=0;j<4;j++)
		{
			s[j]=0;
		}
		
		for(int i=0;i<12;i++) // reading RFID tag number
		{
			id[i]=rx_data();  //assigns 12 rfid numbers to id array
			
			int y=0;
			while (y<4)
			{
				if(a[y][i]==id[i])  // check the received number against the rfid numbers of the students
				{
					s[y]++;  
				}
				y++;
			}
		}
		
		for (int i=0;i<4;i++)
		{
			if(s[i]==12)		// check for authentication and display specific output
			{
				char l[100]="student";
				sprintf(l,"Student %s",name[i]);
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_Write_String(l);
				_delay_ms(50);
				LCD_cmd(0xC0); // move cursor to the start of 2nd line
				_delay_ms(100);
				LCD_Write_String("Attendance Done");
				_delay_ms(4000);
				LCD_cmd(0x01); // make display ON, cursor ON
				_delay_ms(1);
				
				PORTA = PORTA | (1<<6);  // setting demultiplexer for GSM
				// GSM sending message code here
				PORTA = PORTA & (~(1<<6));  // setting demultiplexer for RFID
			}	
		}
}