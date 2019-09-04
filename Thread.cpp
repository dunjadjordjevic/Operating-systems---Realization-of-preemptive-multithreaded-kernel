#include <dos.h>
#include <stdio.h>
#include "Kernel.h"
#include "Thread.h"
#include "SCHEDULE.H"
#include "Pcb.h"
#include "Lista.h"


Thread::Thread(StackSize stackSize, Time timeSlice)
{
	lock
	myPCB = new PCB(stackSize, timeSlice, this); //this je pok na nit koja se kreira
	unlock
}

Thread::~Thread()
{
	lock
	//printf("waitToComplete iz destruktora niti br: %d\n", this->myPCB->getIDofPCB());
	waitToComplete();
	delete myPCB;
	unlock
}

void Thread::waitToComplete()
{
	lock
	//Operacija za cekanje niti
	//nit1.waitToComplete() znaci da se nit koja trenutno
	//radi blokira, dok pozivajuca nit, tj nit1 ne zavrsi sa
	//radom
	//Nema potrebe cekati nit koja je zavrsila vec
	//sa radom, takodje nit ne treba da ceka samu sebe da
	//zavrsi sa radom (dakle ako je ona running nit)

	/*if (((PCB*)Kernel::running)->getIDofPCB()==0)
	{
		printf("Nit koja zove waitToComplete je main nit\n");
	}
	else
	{
		printf("Nit koja zove waitToComplete je:%d u kontekstu niti %d\n", this->myPCB->getIDofPCB(), Kernel::running->myThread->getId());
	}
	*/

	if (myPCB)
	{

		if ((myPCB->state == DONE) || (myPCB==(PCB*)Kernel::running) /*|| (this == Kernel::mainThread) */ || (this == Kernel::idleThread))
		{

			unlock
			return;
		}


		this->myPCB->block(); //ovo poziva dispatch()
	}

	unlock
}

void Thread::start()
{
	lock
	//Ovde PCB ulazi u red spremnih, promena stanja, stek napravljen u konstruktoru

	if (this->myPCB->state==NEW) //ne smem da ubacim nit koja je bila u scheduleru
	{
		this->myPCB->state=READY;
		Scheduler::put(myPCB);
	}

	unlock
}

ID Thread::getId()
{
	return myPCB->getIDofPCB();
}

ID Thread::getRunningId()
{
	//prolaz kroz listu i ona koja ima isRunning=true njen ID vratim

	if (Kernel::listOfPCB->empty()==0)
	{
		//ako nije prazna lista

		Lista::Element* tmp;
		for (tmp=Kernel::listOfPCB->getFirst(); tmp!=0; tmp=tmp->next)
		{
			if (tmp->info->isRunning==1)
			{
				return tmp->info->getIDofPCB();
				//proveri
			}
		}
	}

	return 0;
}

Thread* Thread::getThreadById(ID id)
{

	if (Kernel::listOfPCB->empty()==0)
	{
		//ako nije prazna lista
		Lista::Element* tmp;
		for (tmp=Kernel::listOfPCB->getFirst(); tmp!=0; tmp=tmp->next)
		{
			if (tmp->info->getIDofPCB()==id)
			{
				return tmp->info->myThread;
			}
		}
	}

	return 0;

}


