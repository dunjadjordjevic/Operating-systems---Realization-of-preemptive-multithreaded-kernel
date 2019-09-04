#include "IVTEntry.h"
#include "Pcb.h"
#include<dos.h>

IVTEntry** IVTEntry::IVTable = new IVTEntry*[numberOfEntries];

IVTEntry::IVTEntry(IVTNo n, InterruptRoutine newRoutine)
{
	numberOfEntry = n;
	this->newRoutine = newRoutine;

	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(n);
	setvect(n,newRoutine);
	#endif

	IVTable[numberOfEntry]=this;
	event=0;

}

IVTEntry::~IVTEntry()
{
	lock
	#ifndef BCC_BLOCK_IGNORE
	setvect(numberOfEntry, oldRoutine);
	#endif

	oldRoutine=0;
	newRoutine=0;
	IVTable[numberOfEntry]->event=0;
	unlock
}


IVTEntry* IVTEntry::getEntry(IVTNo n)
{
	return IVTable[n];
	//pazi tamo gde vracas, mozda bude null?
}

void IVTEntry::callOldRoutine()
{
	lock
	(*(this->oldRoutine))();
	unlock

}


void IVTEntry::setEntry(KernelEv* event)
{
	if (event!=0) this->event=event;

}
