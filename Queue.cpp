#include "Pcb.h"
#include "Thread.h"

void Queue::put(PCB* pcb)
{
	//stavlja na kraj

	//printf(" Nit koju ubacujem u waitQueue je %d\n", pcb->getIDofPCB());
	if(first==0)
	{
		first = last = new Elem(pcb);
	}
	else
	{
		Elem* newElem = new Elem(pcb);
		last->next = newElem;
		last=newElem;
	}

	last->next=0;


}

PCB* Queue::get()
{
	//uzima sa pocetka
	if (first != 0)
	{
		Elem* tmpp = first;
		first=first->next;
		tmpp->next=0;
		PCB* tmp = tmpp->info;
		//printf(" Nit koju uzimam iz waitQueue je %d\n", tmpp->info->getIDofPCB());
		return tmp;
	}
	else
	{
		return 0;
	}
}

int Queue::empty()
{
	if (first == 0) return 1;
	return 0;
}

int Queue::numOfElements()
{
	int num=0;

	for (Elem* temp=first; temp!=0;  temp=temp->next) { num++; }

	return num;
}

Queue::~Queue() {


	Elem* tmp = first;
	Elem* old = 0;

	while(tmp != 0)
	{
		old = tmp;
		tmp = tmp->next;
		delete old;
	}

}
