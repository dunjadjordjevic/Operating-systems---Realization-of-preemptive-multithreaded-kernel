#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "Event.h"
#include "Kernel.h"

const unsigned int numberOfEntries = 256;
typedef void interrupt (*InterruptRoutine) (...);
typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry
{

private:


	int numberOfEntry;
	InterruptRoutine oldRoutine, newRoutine;

public:

	KernelEv* event;
	static IVTEntry** IVTable;  //pokazivac na niz ulaza tipa IVTEntry

	static IVTEntry* getEntry(IVTNo n); //dohvata pokazivac na objekat IVTEntry za zadati broj ulaza tabele
	void setEntry(KernelEv* event); // dati ulaz na koji pokazuje this popunjava dogadjajem event
	void callOldRoutine();

	IVTEntry(IVTNo, InterruptRoutine);
	virtual ~IVTEntry();

	//friend class KernelEv;

};




#endif /* H_IVTENTRY_H_ */
