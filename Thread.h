#ifndef _THREAD_H_
#define _THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms

typedef int ID;



class PCB;

class Thread
{

public:
	virtual void start();
	void waitToComplete();
	virtual ~Thread();
	//static void sleep(Time timeToSleep);



	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
	PCB* myPCB;


protected:
	friend class PCB;
	friend class Kernel;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}


};



void dispatch();

#endif
