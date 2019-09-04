/*
 * lista.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#include "Lista.h"
#include <stdio.h>
#include "Pcb.h"
#include "Thread.h"

void Lista::put(PCB* pcb)
{
	//stavlja na kraj
	if(first==0)
	{
		first = last = new Element(pcb);
	}
	else
	{
		Element* newElem = new Element(pcb);

		last->next = newElem;
		last=newElem;

	}


	num++;


}

PCB* Lista::get(int id)
{

	//iz liste izbacuje onaj PCB koji ima dati id, ako takvog nema vraca 0

	Element* tmp = first;
	Element* old = 0;
	Element* result = 0;
	int found=0;

	if (this->empty()==0)
	{
		for (tmp=first; tmp!=0; tmp=tmp->next)
		{
			if (tmp->info->getIDofPCB()==id)
			{
				if (old!=0)
				{
					//tmp nije prvi el liste
					if (tmp==last)
					{
						last=old;
					}

					old->next=tmp->next;


				}
				else
				{
					if (tmp==last)
					{
						//1 el u listi i tmp pokazuje na njega
						last=0;
						first=0;
						old=0;
					}
					else
					{
						first=tmp->next;
						old=0;
					}


				}

				tmp->next=0;
				found=1;
				num--;
				result=tmp;
				delete tmp;
				break;
			}
			else
			{
				old=tmp;
			}
		}

		if (found==0)
		{
			//nije nasao dati element, vraca 0
			return 0;
		}

	}
	else
	{
		//prazna lista
		return 0;

	}

	return tmp->info;

}

int Lista::empty()
{
	if (first == 0) return 1;
	return 0;
}

int Lista::numOfElements()
{

	return this->num;
}

Lista::~Lista() {


	Element* tmp = first;
	Element* old = 0;

	while(tmp != 0)
	{
		old = tmp;
		tmp = tmp->next;
		delete old;
	}
	this->num=0;

}

Lista::Element* Lista::getFirst()
{
	//vraca pokazivac na prvi el liste, i ne brise ga
	return this->first;
}

void Lista::writeAllElements()
{
	/*Element* tmp;

	for (tmp = first; tmp!=0; tmp=tmp->next)
	{
		printf("%d ", tmp->info->getIDofPCB());
	}
	printf("\n");*/

}


PCB* Lista::getElement()
{
	//iz liste brise prvi element, vraca njegov informacioni deo
	Element* tmp=0;
	PCB* tmpp=0;
	if (first!=0)
	{
		tmp = this->first;
		first=first->next;
		tmp->next=0;
		tmpp=tmp->info;
		this->num--;
		delete tmp;

	}

	return tmpp;
}

PCB* Lista::getElementEqualTo(PCB* el)
{
	Element* tmp = this->first;
	Element* beforeTmp =0;
	Element* pom=0;

	for (tmp=this->first; tmp!=0; beforeTmp=tmp, tmp=tmp->next)
	{

		if (tmp->info==el)
		{
			//izbrisi ga iz liste
			beforeTmp->next = tmp->next;
			tmp->next=0;
			this->num--;
			pom=tmp;
			delete tmp;
			return pom->info;
		}
	}

	return 0;
}

