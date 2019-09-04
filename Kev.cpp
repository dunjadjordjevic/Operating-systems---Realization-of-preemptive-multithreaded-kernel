#include "Kev.h"
#include "Event.h"
#include "IVTEntry.h"
#include "Pcb.h"
#include "SCHEDULE.H"

ID KernelEv::lastID=0;

KernelEv::KernelEv(IVTNo n)
{
	id=lastID++;
	value=1;

	//preusmeravanje
	myEntry = IVTEntry::IVTable[n];
	IVTEntry::IVTable[n]->setEntry(this);

	this->creatorOfEvent = (PCB*)Kernel::running;

}

KernelEv::~KernelEv()
{
	//printf("Brisem PCB %d", this->creatorOfEvent->getIDofPCB());
	//delete creatorOfEvent;
	delete myEntry;

}

void KernelEv::wait()
{
	if ((PCB*) Kernel::running == this->creatorOfEvent)
	{
		//ako wait poziva nit koja je vlasnik ovog dogadjaja
		//onda se ona blokira, inace wait nema dejstva

		//printf("\nUsao sam u wait eventa\n");
		if (this->value==1)
		{
			value=0;
			return;
		}

		if (this->value==0)
		{
			//blokiranje
			((PCB*)Kernel::running)->state = BLOCKED;
			((PCB*)Kernel::running)->isRunning = 0;
			Kernel::dispatch();
		}


	}
	else
	{
		return;
	}

}

void KernelEv::signal()
{

	if ((this->value==0) && (this->creatorOfEvent->state==BLOCKED))
	{
		//deblokiranje
		creatorOfEvent->state=READY;
		Scheduler::put(this->creatorOfEvent);

		//printf("\n\nPRITISNUTA TASTATURAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!\n\n");
	}
	else
	{
		//onaj koji je kreirao ovaj dogadjaj nije blokiran na njemu, pa niko i ne moze da se odblokira
		//ili je vr dogadjaja takva da ne treba da se blokira
		value=1;
	}




}

