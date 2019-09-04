/*
 * semlist.h
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */

#ifndef H_SEMLIST_H_
#define H_SEMLIST_H_

class KernelSem;

class SemList
{

public:

	struct Element
	{
		KernelSem* info;
		Element* next;
		Element(KernelSem* i=0, Element* n = 0) { info = i; next=n; }
	};

	SemList():first(0), last(0), num(0) {}
	virtual ~SemList();

	void put(KernelSem*);
	KernelSem* get(int);

	Element* getFirst(); //vraca pokazivac na prvi el
	int empty();
	int numOfElements();

	void writeAllElements();


	int num;
private:

	Element* first, *last;

};




#endif /* H_SEMLIST_H_ */
