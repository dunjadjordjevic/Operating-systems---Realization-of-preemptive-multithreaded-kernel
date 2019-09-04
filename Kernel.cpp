#include<stdio.h>
#include<dos.h>
#include "Kernel.h"
#include "Lista.h"
#include "Thread.h"
#include "SCHEDULE.H"
#include "Sleep.h"
#include "iostream.h"
#include "Pcb.h"
#include "Semlist.h"


InterruptRoutine Kernel::oldTimerRoutine = 0;
volatile PCB* Kernel::running = 0;
IdleThread* Kernel::idleThread = 0;
volatile int Kernel::changeContext=0;
Lista* Kernel::listOfPCB = 0;
SemList* Kernel::allSemaphores= new SemList();
volatile unsigned tbp, tsp, tss;
volatile int counter;
//Thread* Kernel::mainThread=0;

void Kernel::initialize()
{
	lock

#ifndef BCC_BLOCK_IGNORE

	oldTimerRoutine = getvect(0x08);
	setvect(0x08, newTimerRoutine);
	setvect(0x60, oldTimerRoutine);

#endif

	//printf("INITIALIZE!\n");


	listOfPCB = new Lista();

	running = new PCB();
	running->isRunning = 1;
	running->state = READY;

	counter = running->execution_time;

	idleThread = new IdleThread(defaultStackSize, 1);
	idleThread->start();
	idleThread->myPCB->isRunning = 0;

	//mainThread = new Thread(0x10000, 1);
	//mainThread->myPCB->state = READY;

	//running = (volatile PCB*)mainThread->myPCB;
	//main je trenutna running nit


	//allSemaphores = new SemList();

	unlock

}

void Kernel::finalize()
{
	lock
	#ifndef BCC_BLOCK_IGNORE

	setvect(0x08, oldTimerRoutine); //u ulazu 0x08 tabele vracam staru prekidnu rutinu, da ocuvam stanje
#endif


	delete idleThread;
	delete running;
	delete listOfPCB;

	//delete mainThread;

	unlock;
}

void interrupt Kernel::newTimerRoutine(...)
{
	//printf("INTERRUPT\n");



	if (changeContext==0) //Ako nije eksplicitno zahtevena promena konteksta
	{
		//printf("Nije eksplicitno zahtevana promena konteksta. \n");


			if (counter!=0)
			{
				counter--;
				running->left_time--;
				tick();
				updateSleepList();
				//printf("counter se smanjio i sad je: %d\n", counter);
				asm int 60h;

			}
			else
			{
				if (running->left_time==0 && running->execution_time!=0)
				{
					//Neka nit koja nema timeSlice 0 je dosla do kraja svog zivota i zato joj se oduzima procesor

					asm {

						mov tbp, bp
						mov tsp, sp
						mov tss, ss
					}

					running->bp = tbp;
					running->sp = tsp;
					running->ss = tss;



					if (running->state==READY && running!=idleThread->myPCB)
					{
						//printf("%d je stara nit\n",running->id);
						running->isRunning=0;
						Scheduler::put((PCB*) running);
					}
					else
					{
						if (running==idleThread->myPCB && running->state==READY)
						{
							//printf("idle je stara nit\n");
							running->isRunning=0;
						}
					}

					while(1)
					{

						running=(volatile PCB*)Scheduler::get();

						if (running!=0)
						{
							if(running->state == READY)
							{

								break;
							}

						}
						else
						{
							running = (volatile PCB*) idleThread->myPCB;
							running->isRunning = 1;
							//printf("Uzeo sam idle\n");
							break;
						}



					}


					//printf("%d je nova nit\n",running->id);


					tbp = running->bp;
					tsp = running->sp;
					tss = running->ss;

					asm {

						mov bp, tbp
						mov sp, tsp
						mov ss, tss
					}

					counter = Kernel::running->execution_time;
					Kernel::running->left_time=counter;
					running->isRunning=1;
					changeContext = 0;


					asm int 60h;
				}
				else
				if (running->left_time==0 && running->execution_time==0)
				{
					//ako je counter 0, ali je i timeSlice=0 tj radi se o niti koja treba da se izvrsava dok joj nego nasilno
					//ne preotme procesor, samo pozovi 60
					asm int 60h;
					return;
				}


			}


			//poziv stare prekidne rutine, koja je sad na 60h (a bila na 08), samo onda kada nije zahtevana

	}
	else
	{
		//printf("Eksplicitno je zahtevana promena konteksta. \n");

		asm {
			mov tbp, bp
			mov tsp, sp
			mov tss, ss
		}

		running->bp = tbp;
		running->sp = tsp;
		running->ss = tss;




		if (running->state==READY && running!=idleThread->myPCB)
		{
			//ako je stanje BLOCKED pozvan je iz waitToComplete ili iz wait-a kod Semafora
			//printf("%d je stara nit\n",running->id);
			running->isRunning=0;
			Scheduler::put((PCB*) running);
		}
		else
		{
			if (running==idleThread->myPCB)
			{
				//printf("idle je stara nit\n");
				running->isRunning=0;
			}
		}

		while(1)
		{

			running=(volatile PCB*)Scheduler::get();

			if (running!=0)
			{

				if(running->state == READY)
				{

					break;
				}

			}
			else
			{
				running = (volatile PCB*) idleThread->myPCB;

				running->isRunning = 1;
				//printf("Uzimam idle nit \n");
				break;

			}

		}


		//printf("%d je nova nit\n",running->id);


		tbp = running->bp;
		tsp = running->sp;
		tss = running->ss;

		asm {
			mov bp, tbp
			mov sp, tsp
			mov ss, tss
		}

		counter = Kernel::running->execution_time;
		Kernel::running->left_time=counter;
		changeContext = 0;
		Kernel::running->isRunning=1;

	}




}

void Kernel::dispatch()
{
	//fja koja vrsi promenu konteksta niti
	//dakle podaci niti koja je do sada radila se cuvaju
	//a menjaju se podacima one niti koja je izabrana iz Schedulera kao sl nit koja
	//se izvrsava umesto nje
	lock
	//printf("Nit koja je usla u dispatch je: %d \n", running->id);

	changeContext = 1; //dispatched
	asm int 08h;
	unlock
}


Lista* Kernel::getListOfPcb()
{
	return listOfPCB;
}

Kernel::~Kernel(){



}

SemList* Kernel::getListOfSemaphores()
{
	return allSemaphores;
}

void Kernel::updateSleepList()
{
	//prolaz kroz sve semafore iz liste allSemaphores
	//za svaki od njih azurira sleepList

	//NE ZABORAVI da postavis deblockedOnTime fleg za nit koju odblokiras na semaforu

	//printf("UPDATE SLEEP LISTE!\n");
	SemList::Element* sem;
	for (sem=Kernel::allSemaphores->getFirst(); sem!=0; sem=sem->next)
	{

		SleepList::Elem* tmp;
		/*if (sem->info->sleepList->getHead()!=0)
		{
			//printf("Ima nesto u sleep listi za dati sem!\n");
			sem->info->sleepList->writeList();
		}
		else
		{
			//printf("Nema nista u sleep listi semafora\n");
		}*/

		tmp = sem->info->sleepList->getHead();
		//tmp pokazuje na prvi element liste sleep semafora sem

		if (tmp!=0)
		{
			if (tmp->info!=0)
			{
				tmp->info--;
				if (tmp->info == 0)
				{
					// prodjem kroz ostatak liste, do prvog koji nije 0 i one posle mene koji su 0 izbacim iz liste

					SleepList::Elem* pointer = tmp;
					SleepList::Elem* old = 0;
					while (pointer != 0 && pointer->info == 0)
					{
						//odblokiraj onog na koji pokazuje pointer
						old = pointer;
						pointer = pointer->next;
						tmp = pointer;
						sem->info->sleepList->deblockThread(old, 1);
						sem->info->increaseValue(); //taj neko vise nije blokiran na semaforu

					}

					tmp = 0;
					//sleep lista je update-ovana
				}
			}
			else
			{
				SleepList::Elem* pointer = tmp;
				SleepList::Elem* old = 0;
				while (pointer != 0 && pointer->info == 0)
				{
					//odblokiraj onog na koji pokazuje pointer
					old = pointer;
					pointer = pointer->next;
					tmp = pointer;
					sem->info->sleepList->deblockThread(old, 1);
					sem->info->increaseValue(); //taj neko vise nije blokiran na semaforu

				}

					tmp = 0;

			}
		}


		/*if (sem->info->sleepList->getHead() != 0)
		{
			//printf("STANJE POSLE UPDATEa\n");
			sem->info->sleepList->writeList();
		}
		else
		{
			//printf("STANJE POSLE UPDATEa: Nema nista u sleep listi semafora\n");
		}*/

	}





}

void Kernel::writeListOfSemaphores()
{
	/*
	SemList::Element* tmp;

	for (tmp=allSemaphores->getFirst(); tmp!=0; tmp=tmp->next)
	{
		//printf("%d ", tmp->info->getIDofSem());
	}
	printf("\n");
	 */
}

void Kernel::tick(){

}
