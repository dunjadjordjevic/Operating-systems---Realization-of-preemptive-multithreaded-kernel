/*
 * idle.h
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_
#include <stdio.h>
#include "Thread.h"


class IdleThread: public Thread {
public:


	IdleThread(StackSize stackSize=defaultStackSize , Time timeSlice=defaultTimeSlice) : Thread(stackSize, timeSlice){}

	~IdleThread() { waitToComplete(); }

	void run ()
	{

		while (1);
	}

	void start();


};




#endif /* H_IDLE_H_ */
