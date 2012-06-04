#include <stddef.h>



/*
 * coord.h
 *
 * Created: 21/10/2011 7:46:58 AM
 *  Author: Abhshkrv
 */ 


#ifndef COORD_H_
#define COORD_H_

/* ======================================= */
/* Configuration Flags					   */
/* Comment out to prevent inclusion of     */
/* feature                                 */
/* ======================================= */
#define OSIncludeQueues
#define OSIncludeSema	

// Most RTOS have a single common structure for semaphores, queues, mailboxes, mutexes, etc. The trick is to
// create a structure that can accomodate features of all the various services but still be minimal in size. :)
typedef struct osstruct
{
	int element;
	struct osstruct* next;
	
} OSSignal;
//int tail_index=-1;
int total_size=-1;
//int head_index=-1;
int size_of_queue=-1;
struct osstruct* head = NULL;
struct osstruct* tail = NULL;

//struct osstruct sema;

/* ======================================= */
/* Critical section management             */
/* ======================================= */
void enterCritical();
void leaveCritical();

/* ======================================= */
/* Semaphore routines                      */
/* ======================================= */

// Creates a semaphore with initial value initval
OSSignal *OSmakeSema(int initval);

// Takes a semaphore. Blocks if semaphore is 0.
void OSSemPend(OSSignal *sema);

// Posts a semaphore.
void OSSemPost(OSSignal *sema);


/* ======================================= */
/* Queue routines                          */
/* ======================================= */

// qsize=number of elements in the queue. esize=size of each element. So calling makeQ(10, sizeof(int)) would produce a queue of 10 elements, 
// with each element being the size of an integer.
OSSignal *OSmakeQ(int qsize, int esize);

// Inserts item into queue q
void OSenq(OSSignal *q, void *item);

// Removes and returns item from head of queue. Process blocks if queue is empty.
void *OSdeq(OSSignal *q);

// Destroys the queue
void *OSdestroyQ(OSSignal *q);


#endif /* COORD_H_ */
#include "coord.c"