#include <dos.h>
#include <stdio.h>
#include "Kernel.h"
#include "SCHEDULE.H"
#include "Lista.h"
#include "iostream.h"
#include "Pcb.h"
#include "Queue.h"
#include "Thread.h"

ID PCB::lastID=0;

void PCB::block()
{
	lock
	Kernel::running->state = BLOCKED;
	Kernel::running->isRunning=0;

	waitQueue->put((PCB*)Kernel::running);

	Kernel::dispatch();
	unlock
}

PCB::PCB()
{


	lock

	this->execution_time = 0; //kad je timeslice 0, znaci da ona treba da se izvrsava sve dok se ne zatrazi eksplicitna promena
	this->left_time=0;
	this->myThread=0;
	this->isRunning=0;
	this->deblockedOnTime =0;
	stack=0;

	//printf("LastID je : %d \n", lastID);
	id = lastID++;
	waitQueue=new Queue();
	state = NEW;
	Kernel::listOfPCB->put(this);
	unlock
}

PCB::PCB(StackSize stackSizePcb, Time execution_time, Thread* myThread)
{
	lock

		this->stackSizePcb= stackSizePcb;
		this->execution_time = execution_time;
		this->left_time=execution_time;
		this->myThread = myThread;
		this->deblockedOnTime =0;

		stackSizePcb /= sizeof(unsigned);

		stack = new unsigned[stackSizePcb];

		stack[stackSizePcb-5] =0x200;//setovan I fleg u
										// pocetnom PSW-u za nit
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSizePcb-1] = FP_SEG(this);
		stack[stackSizePcb-2] = FP_OFF(this);
		stack[stackSizePcb-6] = FP_SEG(PCB::wrapper);
		stack[stackSizePcb-7] = FP_OFF(PCB::wrapper);
		bp = sp = FP_OFF(stack+stackSizePcb-16);
		ss = FP_SEG(stack+stackSizePcb-16);
	#endif
	unlock

	//printf("LastID je : %d\n", lastID);

	id = lastID++;
	this->isRunning=0;
	waitQueue=new Queue();
	state = NEW;
	Kernel::listOfPCB->put(this);

}

void PCB::wrapper(PCB *pcb)
{

	pcb->myThread->run();

	lock

	pcb->state = DONE;

	PCB *temp;
	while (!pcb->waitQueue->empty())
	{
		//dok red onih koji cekaju da se myThread zavrsi ne postane prazan, ti sve niti odblokiraj

		//printf("Broj elemenata u redu cekanja je: %d\n", pcb->waitQueue->numOfElements());
		temp = pcb->waitQueue->get();

		if (temp != 0) {


		temp->state = READY;


		//printf("Odblokirao sam nit blokiranu nad %d ciji je id: %d \n", pcb->myThread->myPCB->getIDofPCB(), temp->getIDofPCB());

		Scheduler::put(temp);

		}

	}
	// odblokira sve niti koje su blokirane nad ovoj niti, one koje su cekale da se myThread zavrsi i sad
	//postaju ready, tj idu u rasporedjivac

	Kernel::dispatch();
	unlock
}

PCB::~PCB()
{
	lock

	delete [] stack;
	delete waitQueue;
	PCB* tmp= Kernel::listOfPCB->get(this->id);

	unlock

}

ID PCB::getIDofPCB()
{
	return this->id;
}



