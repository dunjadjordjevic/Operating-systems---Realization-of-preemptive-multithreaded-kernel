/*
 * sleep.h
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */

#ifndef H_SLEEP_H_
#define H_SLEEP_H_

extern long abs(long);
typedef unsigned int Time;
class PCB;

class SleepList
{



public:

	struct Elem
	{
		PCB* pcb;
		Time info;
		Elem* next;
		Elem(Time x, PCB* p) { this->info=x; next= 0; pcb=p; }
	};


	SleepList();
	virtual ~SleepList();


	void put(Time timeToSleep, PCB* p);
	void deblockThread(Elem* pointer, int par);
	void writeList();

	int getNumOfElem();

	Elem* getHead();

private:

	int num; //br el sleep liste
	Elem *first;
};


#endif /* H_SLEEP_H_ */
