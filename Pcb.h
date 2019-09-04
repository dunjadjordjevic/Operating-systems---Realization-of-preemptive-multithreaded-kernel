
#ifndef _PCB_H_
#define _PCB_H_

#include "Lista.h"
#include "Queue.h"
#include "Thread.h"
// #include <dos.h>
// #include <iostream.h>


typedef enum { NEW, READY, BLOCKED, DONE} State; // stanje niti

//lock i unlock
#define lock asm pushf; asm cli;
#define unlock asm popf;


class PCB
{
private:

	ID id;
	static ID lastID;

	StackSize stackSizePcb;

	unsigned *stack;
	unsigned ss;

	unsigned sp;
	unsigned bp;

	Time execution_time; //pocetni time_slice, velicina koja se ne menja


	Queue *waitQueue; //pok na red za cekanje niti

	static void wrapper(PCB* pcb);

	Thread* myThread;


public:

	int isRunning;
	int deblockedOnTime;
	volatile State state;
	Time left_time; //velicina koja se menja i ide ka nuli

	PCB(StackSize, Time, Thread*);
	PCB();
	virtual ~PCB();
	void block();

	ID getIDofPCB();

	friend class KernelSem;
	friend class Kernel;
	friend class Thread;
	friend class List;
	friend class IdleThread;

};



#endif
