/*
 * semlist.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */
#include <stdio.h>

#include "Kernel.h"
#include "Semlist.h"
#include "Ksem.h"

 SemList::~SemList()
 {
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

 void SemList::put(KernelSem* s)
 {
	 if(first==0)
	 {
	 	first = last = new Element(s);
	 	//printf("Ubacio sam element kao prvi u listu semafora\n");
	 }
	 else
	 {
	 	Element* newElem = new Element(s);
	 	last->next = newElem;
	 	last=newElem;
	 	//printf("Ubacio sam element u listu semafora\n");
	 }

	 last->next=0;
	 num++;

	// Kernel::writeListOfSemaphores();

 }

 KernelSem* SemList::get(int id)
 {

	Element* tmp = first;
	Element* result=0;
	Element* old = 0;
	int found=0;

	if (this->empty()==0)
	{
		for (tmp=first; tmp!=0; tmp=tmp->next)
		{
			if (tmp->info->getIDofSem()==id)
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

	return result->info;

 }

 int SemList::empty()
 {
 	if (first == 0) return 1;
 	return 0;
 }

 int SemList::numOfElements()
 {

 	return this->num;
 }

 SemList::Element* SemList::getFirst()
 {
 	return this->first;
 }
