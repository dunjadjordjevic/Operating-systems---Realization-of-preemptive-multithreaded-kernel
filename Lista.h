/*
 * List
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef LISTA_
#define LISTA_

#include "Kernel.h"
#include "Pcb.h"

//static int id = 0;

class Thread;

class Lista
{
public:


	struct Element
	{
		PCB* info;
		Element* next;
		Element(PCB* i=0, Element* n = 0) { info = i; next=n; }
	};

	Lista():first(0), last(0), num(0) {}
	virtual ~Lista();

	void put(PCB*);
	PCB* get(int);

	Element* getFirst();
	int empty();
	int numOfElements();

	void writeAllElements();

	PCB* getElement();
	PCB* getElementEqualTo(PCB* el);


	int num;
private:

	Element* first, *last;
	friend class Thread;

};



#endif /* LISTA_ */
