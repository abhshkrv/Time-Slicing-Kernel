/*
 * kernel.h
 *
 * Created: 10/9/2011 12:51:20 PM
 *  Author: Abhshkrv
 */ 


#ifndef KERNEL_H_
#define KERNEL_H_

// ------------- Customize Below ----------------- //
#define OS_NUM_TASKS 4

// OS Scheduler Type
// -1 - Not set
// 0 - Fixed
// 1 - RMS
// 2 - EDF

#define OS_SCHED_TYPE   -1
// ------------- Customize Above ----------------- //

void OSSwapTask();
void OSInit();
int OSAddTask(void (*taskptr)(void *), int prio, void* arg);
void OSRun();
void OSMakeAtomic();
void OSLeaveAtomic();

#endif /* KERNEL_H_ */
