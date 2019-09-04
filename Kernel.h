
#ifndef _KERNEL_H_
#define _KERNEL_H_
#include <dos.h>
#include "Idle.h"
#include "Kev.h"
#include "Ksem.h"
#include "Lista.h"
#include "Pcb.h"
#include "Thread.h"


typedef int ID;
typedef unsigned long StackSize;
typedef unsigned int Time;
typedef void interrupt (*InterruptRoutine) (...);

class SemList;
class Lista;
class PCB;
class Thread;
class IdleThread;

class Kernel
{
public:
	static void dispatch();
	static void initialize();
	static void finalize();
	static void tick();
	static void updateSleepList();
	static Lista* getListOfPcb();
	static SemList* getListOfSemaphores();
	static void writeListOfSemaphores();
	static SemList* allSemaphores;
	~Kernel();

private:
	friend class PCB;
	friend class Thread;
	friend class SemList;
	friend class KernelSem;
	friend class SleepList;
	friend class KernelEv;

	static InterruptRoutine oldTimerRoutine;
	static Lista* listOfPCB;
	static void interrupt newTimerRoutine (...);
	static volatile int changeContext, forbidPreemption;
	static volatile PCB *running;


	static IdleThread* idleThread;
	//static Thread* mainThread; //pok na main nit


};

#endif
