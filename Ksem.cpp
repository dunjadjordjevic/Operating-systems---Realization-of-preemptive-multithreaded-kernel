#include "Pcb.h"
#include "SCHEDULE.h"
#include "Sleep.h"
#include "Kernel.h"
#include "Ksem.h"
#include "Ksem.h"
#include "Queue.h"
#include "Semaphor.h"
#include "Semlist.h"
ID KernelSem::lastID=0;




KernelSem::KernelSem(int init)
{
	this->value=init;
	id = lastID++;
	Kernel::allSemaphores->put(this);
	blockedOnSem = new Queue();
	sleepList = new SleepList ();
}

KernelSem::~KernelSem ()
{
	KernelSem* ks = Kernel::getListOfSemaphores()->get(this->getIDofSem());
	delete blockedOnSem;
	delete sleepList;
}


int KernelSem::signal(int n)
{

	int result;
	PCB* tmp;
	int rest;

	if (n<0)
	{
		//nema dejstvo
		result = n;
	}
	else
		{

			//n>0
			//treba odblokirati n niti ako ih je toliko blokirano
			//u suprotnom odblokirati toliko koliko je odblokirano i taj br vratiti

			int numOfDeblocked=0;
			int counter=0;
			value=value+n;

			if (blockedOnSem->numOfElements()>=n)
			{
				//ako ima bar n niti u redu blokiranih (onih gde je vreme INF), njih odblokiram
				counter=n;
				for (int i=0; i<counter; i++)
				{
					tmp = blockedOnSem->get();
					tmp->deblockedOnTime=0;
					tmp->state=READY;
					Scheduler::put(tmp);
					numOfDeblocked++;

				}
			}
			else
			{
				counter=blockedOnSem->numOfElements();
				rest = n-counter; // broj onih koje treba da odblokiram iz sleep liste

				for (int i=0; i<counter; i++)
				{
					tmp = blockedOnSem->get();
					tmp->deblockedOnTime=0;
					tmp->state=READY;
					Scheduler::put(tmp);
					numOfDeblocked++;

				}

				counter= n-counter;
				SleepList::Elem* t;

				//Ako je signal pozvan da odblokira 1 ili vise niti, pri cemu kad njih odblokiram ostaju nule
				//prvu nulu u nizu nula treba staviti da ima vrednost onoga sto sam prvo odblokirala iz sleep liste

				SleepList::Elem* keeper;
				if (sleepList->getNumOfElem()>=counter)
				{
					for (int i=0; i<counter; i++)
					{
						if (sleepList->getHead()!=0 && sleepList->getHead()->info!=0) keeper->info= sleepList->getHead()->info;
						if (i!=counter-1)
						{
							sleepList->deblockThread(this->sleepList->getHead(), 0);
							numOfDeblocked++;
						}
						else
						{
							if (sleepList->getHead()->next!=0)
							{
								if (sleepList->getHead()->next->info==0)
								{
									sleepList->getHead()->next->info=keeper->info;
								}
								sleepList->deblockThread(this->sleepList->getHead(), 0);

							}
							else
							{
								sleepList->deblockThread(this->sleepList->getHead(), 0);

							}
							numOfDeblocked++;


						}


					}
				}
				else
				{
					counter = sleepList->getNumOfElem();

					for (int i=0; i<counter; i++)
					{
						if (sleepList->getHead()!=0 && sleepList->getHead()->info!=0) keeper->info= sleepList->getHead()->info;
						if (i!=counter-1)
						{
							sleepList->deblockThread(this->sleepList->getHead(), 0);
							numOfDeblocked++;
						}
						else
						{
							if (sleepList->getHead()->next!=0)
							{
								if (sleepList->getHead()->next->info==0)
								{
									sleepList->getHead()->next->info=keeper->info;
								}
								sleepList->deblockThread(this->sleepList->getHead(), 0);

							}
							else
							{
								sleepList->deblockThread(this->sleepList->getHead(), 0);

							}
							numOfDeblocked++;


						}


					}

				}


			}

			result=numOfDeblocked;
		}


	return result;

}

int KernelSem::wait(Time maxTimeToWait)
{
	//maxTime je max trajanje blokiranja pozivajuce niti


	int result;

	value--;
	if (maxTimeToWait==0)
	{
		//nit ceka dok se ne desi signal

		if (value>=0)
		{
			result=1;
		}
		else
		{
			//blokira nit na ovom semaforu, ali ne stavlja u sleep listu vec u blockedOnSem queue

			Kernel::running->state = BLOCKED;
			Kernel::running->isRunning=0;
			blockedOnSem->put((PCB*)Kernel::running);
			//printf("Stavio sam nit %d u red blokiranih na semaforu %d\n", ((PCB*)Kernel::running)->getIDofPCB(), this->getIDofSem());
			Kernel::dispatch();
			result = Kernel::running->deblockedOnTime;

		}

	}
	else
	{
		if (value>=0)
		{
			result = 1;
		}
		else
		{

			Kernel::running->state = BLOCKED;
			Kernel::running->isRunning=0;
			//printf("U sleep listu na semaforu %d ide nit %d:\n", this->getIDofSem(), ((PCB*)Kernel::running)->getIDofPCB());
			this->sleepList->put(maxTimeToWait, (PCB*)Kernel::running);
			Kernel::dispatch();
			result = Kernel::running->deblockedOnTime;
		}

	}

	return result;
}

int KernelSem::val() const
{
	return this->value;
}

int KernelSem::getIDofSem()
{
	return this->id;
}

SleepList::Elem* KernelSem::getFirstInSleepList()
{
	return this->sleepList->getHead();
}

void KernelSem::increaseValue()
{
	this->value++;
}
void KernelSem::decreaseValue()
{
	this->value--;
}
