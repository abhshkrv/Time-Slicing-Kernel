/*
 * serial.h
 *
 * Created: 16/10/2011 7:55:48 PM
 *  Author: Abhshkrv
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

// Maximum outstanding characters for sending to the PC. If queue is full characters are lost.
#define QLEN	64

// Maximum outstanding characters in receive queue. If queue is full characters are lost.
#define LQLEN	128

// Call this to setup the serial port for 9600 8N1, and to initialize the say and hear queues.
void setupSerial();

// Say a string to the terminal on the PC/notebook from Arduino
void say(char *str);

// Hear something sent from the PC/notebook to the Arduino. len=0 if nothing was sent, otherwise contains # of characters received.
void hear(char *str, int *len);

#endif /* SERIAL_H_ */