#include "Event.h"
#include "Pcb.h"
#include "Kev.h"

Event::Event(IVTNo ivtNo)
{
	lock
	myImpl = new KernelEv(ivtNo);
	unlock
}

Event::~Event()
{
	lock
	delete myImpl;
	unlock
}

void Event::wait()
{
	lock
	myImpl->wait();
	unlock
}

void Event::signal()
{
	lock

	if (myImpl!=0) myImpl->signal();

	unlock
}


