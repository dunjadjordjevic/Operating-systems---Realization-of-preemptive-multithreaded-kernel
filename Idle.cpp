/*
 * idle.cpp
 *
 *  Created on: Jul 10, 2019
 *      Author: OS1
 */
#include<stdio.h>
#include<dos.h>

#include "Idle.h"
#include "Pcb.h"
#include "Thread.h"
#include "Kernel.h"

void IdleThread::start()
{

	//Nit ne treba staviti u Scheduler
	lock

	if (myPCB->state==NEW)
	{
		myPCB->state=READY;

	}

	unlock
}

