/*
 * sleep.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */

#include <stdio.h>
#include "Pcb.h"
#include "Sleep.h"
#include "Thread.h"
#include "SCHEDULE.h"


SleepList::SleepList()
{
	first = 0;
	num=0;
}

void SleepList::put(Time timeToSleep, PCB* pcb)
{

	Elem* newNode;
	Elem* tmp = first;
	Time t=timeToSleep;

	newNode = new Elem(timeToSleep, pcb);
	if (first==0)
	{
		//Prazna lista, samo me stavi u nju
		first = newNode;
		tmp=0;
	}
	else
	{
		while (tmp!=0)
		{
			//Ima vec nesto u listi
			if (tmp->info > newNode->info)
			{
				//nova nit hoce manje da spava od postojece , ubaci se nova, postojeca spava isto kao nova + dodatno vreme razlike
				newNode->info = t;
				newNode->next = tmp;
				first = newNode;
				t = t > tmp->info ? t - tmp->info : tmp->info - t;
				tmp->info = t;
				tmp=0;
			}
			else
			if (tmp->info < newNode->info)
			{
				//nova nit hoce da spava vise od postojece
				//prvi el ne diraj, newNode->info dobija vr tmp->info-timeToSleep, gde je tmp=head, pa se pita i za ostale clanove
				t = t > tmp->info ? t - tmp->info : tmp->info - t;
				newNode->info = t;
				if (tmp->next!=0)
					tmp = tmp->next;
				else
				{
					tmp->next=newNode;
					tmp=0;
				}
			}
			else
			{
				// situacija tmp->info=timeToSleep
				//tmp pokazuje na posl el koji je isti kao timeToSleep, ako ih ima vise istih pr 5 2 1 1 ubacujem 1
				//iza njega treba da ubacim
				if (tmp->next!=0)
				{
					t =0;
					newNode->info = t;
					while((tmp->next!=0) && ((tmp->next)->info==0))
					{
						tmp=tmp->next;
					}
					//tmp pokazuje na posl el koji ima info 0 i ubacuje iza njega newNode
					if (tmp->next!=0)
					{
						newNode->next=tmp->next;
						tmp->next = newNode;
						tmp=0;
					}
					else
					{
						newNode->next = tmp->next;
						tmp->next=newNode;
						tmp=0;
					}
				}
				else
				{
					t =0;
					newNode->info = t;
					tmp->next=newNode;
					newNode->next=0;
					tmp=0;
				}
			}
		}
	}
	num++;


	//this->writeList();

}

SleepList::Elem* SleepList::getHead()
{
	return first;
}

SleepList::~SleepList() {


	Elem* tmp = first;
	Elem* old = 0;

	while(tmp != 0)
	{
		old = tmp;
		tmp = tmp->next;
		delete old;
	}

	delete first;

}

void SleepList::writeList()
{

	/*Elem* tmp;
	printf("\n");
	printf("Elementi liste sada su: \n");


	if (first!=0)
	{
		for (tmp=first; tmp!=0; tmp=tmp->next)
		{
			printf("%d ", tmp->info);
		}
		printf("\n");
	}
	else
	{
		printf("Nema nista u sleep listi\n");
	}
*/



}

int SleepList::getNumOfElem()
{
	return num;
}

void SleepList::deblockThread(Elem* s, int par)
{
	/*
		deblokirati jedan element i azurirati listu!!!
		deblokirati - izbaciti iz liste taj el, i PCB te niti odblokirati, stavljanjem stanja na READY
		i ubacivanjem u Scheduler
	*/

	Elem* p = s;
	Elem* f = this->first;

	f=f->next;
	this->first=f;
	p->next=0;
	p->pcb->state=READY;
	//printf("Izbacio sam iz sleep liste nit %d i ubacio u Scheduler\n", p->pcb->getIDofPCB());
	Scheduler::put(p->pcb);
	p=0;
	s=0;
	num--;


	if (par == 0)
	{
		//pozvano iz signal, znaci odblokiraj 1 nit
		//deblockedOnTime=0

		p->pcb->deblockedOnTime = 0;


	}
	else
	if (par==1)
	{
		//pozvano iz updateSleepList()
		p->pcb->deblockedOnTime = 1;

	}



}

