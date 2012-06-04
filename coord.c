/*
 * coord.c
 *
 * Created: 21/10/2011 8:36:30 AM
 *  Author: Abhshkrv
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdlib.h>

#ifndef NULL
#define NULL (void*)0
#endif




/* ======================================= */
/* Critical section management             */
/* ======================================= */
inline void enterCritical()
{
	cli();
	//OSSemPend(sema1);
}

inline void leaveCritical()
{
	sei();
	//OSSemPost(sema1);
}

/* ======================================= */
/* Semaphore routines                      */
/* ======================================= */

#ifdef OSIncludeSema


// Creates a semaphore with initial value initval
OSSignal *OSmakeSema(int initval)
{
	OSSignal* sema1 = malloc(sizeof(OSSignal));
	sema1->element = initval;
	return sema1;
}

// Takes a semaphore. Blocks if semaphore is 0.
void OSSemPend(OSSignal *sema)
{
	while(sema->element == 0);
	
	sema->element--;
	
}

// Posts a semaphore.
void OSSemPost(OSSignal *sema)
{
	sema->element++;
}


#endif

/* ======================================= */
/* Queue routines                          */
/* ======================================= */

#ifdef OSIncludeQueues

OSSignal queue;
// qsize=number of elements in the queue. esize=size of each element. So calling makeQ(10, sizeof(int)) would produce a queue of 10 elements, 
// with each element being the size of an integer.
OSSignal *OSmakeQ(int qsize, int esize)
{
	//if(queue != (OSSignal)NULL)
	{
//		enterCritical();
		total_size = qsize;
		size_of_queue = 0;
		head = NULL;
		tail = NULL;
		total_size = qsize;
	//	leaveCritical();
		return &queue;
	}
	//else
		//return NULL;
	//
}

// Inserts item into queue q
void OSenq(OSSignal *q, void *item)
{
	while(size_of_queue==total_size);
	enterCritical();
		if(head == NULL || size_of_queue==0)
		{
			head = q;
			tail = q;
			head->next = NULL;
			tail->next = NULL;
			head->element=(int)item;
		}
		else
		{
			OSSignal* temp = malloc(sizeof(OSSignal));
			tail->next = temp;
			tail = temp;
			tail->element=(int)item;
		}
		
		size_of_queue++;
		leaveCritical();
	
}

// Removes and returns item from head of queue. Process blocks if queue is empty.
void *OSdeq(OSSignal *q)
{
	
	void* toReturn = NULL;
	//while(!size_of_queue || head==NULL);
	if(!size_of_queue || head==NULL)
	return NULL;
		enterCritical();
	toReturn = (void*)(head->element);
	if(size_of_queue==1)
	{
			
			free(head);
			head = NULL;
			tail = NULL;
	}
	else
	{
			head = head->next;		
	}
	size_of_queue--;
	leaveCritical();
	return toReturn;
}

// Destroys the queue
void *OSdestroyQ(OSSignal *q)
{
	OSSignal *temp;
	while(head!=NULL)
	{
			temp = head;
			head=head->next;
			free(temp);
	}
}


#endif