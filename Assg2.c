/*
 * Assg2.c
 *
 * Created: 20/10/2011 1:30:57 PM
 *  Author: Abhshkrv
 */ 

#include <avr/io.h>
#include "coord.h"
#include "serial.h"
#include "kernel.h"

#define NULL (void *) 0
OSSignal *q;
int ctr=0;
struct osstruct* sema;//mutex
struct osstruct* sema2;//empty
struct osstruct* sema3;//full

int noOfIncrements = 0;
// Tasks 1 and 2 will increment a global variable ctr, and every 10 increments task 1 will send the resulting value in ctr to task3 via a queue.
// Task 3 will then send it via serial to the PC. 

// In addition, do something fun and creative with this program, the serial port and the components you have. Describe it in your report. :)
void task1(void *p)
{
	while(1)
	{
		OSSemPend(sema);
		while(noOfIncrements<10)
		{
			enterCritical();
			ctr++;
			noOfIncrements++;
			leaveCritical();
		}		
		if(noOfIncrements >= 10)
		{
			noOfIncrements=0;
			//cli();
			
			//OSSemPend(sema2);
			//enterCritical();
			//OSSemPend(sema);
			
			OSenq(q,(void*)ctr);
			
			//OSSemPost(sema);
			//leaveCritical();
			//OSSemPost(sema3);
			
			//sei();
			//ctr=0;

		}			
		OSSemPost(sema);
	}
	
}

void task2(void *p)
{
	while(1)
	{
		OSSemPend(sema);
		while(noOfIncrements==10);
		enterCritical();
		ctr++;
		noOfIncrements++;
		leaveCritical();
		OSSemPost(sema);
	}
	
}

void task3(void *p)
{
	int t;
	char str[10];
	int size = 0;
	while(1)
	{	
		OSSemPend(sema);
		
		//OSSemPend(sema3);
		//enterCritical();
		//OSSemPend(sema);
		
		t = (int)OSdeq((q));
		
		//OSSemPost(sema);
		//leaveCritical();
		//OSSemPost(sema2);
		
		size = 0;
		while(t)
		{
			str[size++]=(char)((t%10)+48);
			t/=10;
		}		
		
		str[size]='\0';
		/*char temp;
		for(int i=0;i<size/2;i++)
		{
		temp = str[i];
		str[i]=str[size-1-i];
		str[size-1-i]=temp;					
		}*/
		
		say(str);
		//size=0;
		//str[size] = '\0';
		//say(" ");
		OSSemPost(sema);
	}
}


void setup()
{
	setupSerial();
	q=OSmakeQ(100,sizeof(int));
	sema = OSmakeSema(1);
	
	sema2 = OSmakeSema(total_size);
	
	sema3 = OSmakeSema(0);
	
	
}

int main(void)
{
	OSInit();
	setup();
	OSAddTask(task1,0,NULL);
	OSAddTask(task2,1,NULL);
	OSAddTask(task3,2,NULL);
	OSRun();
}