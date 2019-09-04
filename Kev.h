#ifndef H_KEV_H_
#define H_KEV_H_
#include "Event.h"
#include "IVTEntry.h"



//interruptFlag=1 treba zvati staru prekidnu rutinu
//inace ne treba

#define PREPAREENTRY(numOfEntry, interruptFlag)\
void interrupt intt##numOfEntry(...);\
	IVTEntry newEntry##numOfEntry(##numOfEntry, intt##numOfEntry);\
	void interrupt intt##numOfEntry(...){\
		if (newEntry##numOfEntry.event != 0) newEntry##numOfEntry.event->signal();\
			if (interruptFlag==1) newEntry##numOfEntry.callOldRoutine();\
			Kernel::dispatch();\
			}





typedef int ID;


class PCB;
class IVTEntry;

class KernelEv
{

private:

	ID id;
	static ID lastID;
	int value;
	PCB* creatorOfEvent;
	IVTEntry* myEntry;


public:

	friend class Event;
	friend class Kernel;
	friend class PCB;
	friend class IVTEntry;

	KernelEv(IVTNo n);
	virtual ~KernelEv();

	void wait();
	void signal();




};


#endif /* H_KEV_H_ */
