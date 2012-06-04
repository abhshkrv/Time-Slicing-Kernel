/*
 * serial.c
 *
 * Created: 10/6/2011 10:33:22 AM
 *  Author: Abhshkrv
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "serial.h"

// Say queue
char q[QLEN];
int qcount, qhead, qtail;


// Listen queue
char lq[QLEN];
int lqcount, lqhead, lqtail;

#define enter_atomic() cli()
#define leave_atomic() sei()

// Set up for 9600 8N1
void setupSerial()
{
	enter_atomic();
	qcount=0;
	qhead=0;
	qtail=0;
	lqhead=0;
	lqtail=0;
	lqcount=0;

	// Set up baud rate generator. UBBR=16000000/(16*9600)-1 = 103
	UBRR0L=103;
	UBRR0H=0;
	
	// Set up 8N1
	UCSR0C=0b110;
	
	// Enable RX and TX.
	UCSR0B=0b10011000;
	UCSR0A=0;
	leave_atomic();
}

// ISR for transmit end.
ISR(USART_TX_vect)
{

	enter_atomic();
	// Disable interrupts. 
	if(qcount==0)
		UCSR0B&=0b10111111;
	else
	{
		UDR0=q[qtail];
		qtail=(qtail+1)%QLEN;
		qcount--;
	}	
	leave_atomic();
}

// Listener
ISR(USART_RX_vect)
{
	// Guarantee atomicity
	enter_atomic();
		
	// Get the character
	char ch=UDR0;
	if(lqcount<LQLEN)
	{
		lq[lqhead]=ch;
		lqhead=(lqhead+1)%LQLEN;
		lqcount++;
	}	
	
	leave_atomic();
}

void hear(char *str, int *len)
{
	int i;
	
	enter_atomic();
	for(i=0; i<lqcount; i++)
	{
		str[i]=lq[lqtail];
		lqtail=(lqtail+1)%LQLEN;
	}	
	
	str[lqtail]=0;
	
	*len=lqcount;
	lqcount=0;
	
	leave_atomic();
}

void say(char *str)
{
	int i;
	
	// Guarantee atomicity
	enter_atomic();
	for(i=0; i<strlen(str) && qcount < QLEN; i++)
	{
		q[qhead]=str[i];
		qhead=(qhead+1) %QLEN;
		qcount++;			
	
		if(!(UCSR0B & 0b01000000))
		{
			UDR0=q[qtail];
			qtail=(qtail+1)%QLEN;
			qcount--;
			UCSR0B|=0b01000000;
		}
	}
	leave_atomic();
}

