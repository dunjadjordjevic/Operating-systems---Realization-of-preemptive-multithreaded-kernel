#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "Pcb.h"

class Queue
{
private:
	struct Elem
	{
		PCB* info;
		Elem* next;
		Elem(PCB* i=0, Elem* n = 0) { info = i; next=n; }
	};
	Elem* first, *last;
public:

	Queue():first(0), last(0) {}
	virtual ~Queue();

	void put(PCB*);
	PCB* get();

	int empty();
	int numOfElements();
};
#endif
