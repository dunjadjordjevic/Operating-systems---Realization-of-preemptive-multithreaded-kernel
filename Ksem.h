#ifndef _ksem_h_
#define _ksem_h_
#include "Sleep.h"

typedef unsigned int Time;
typedef int ID;

class Queue;
class SleepList;


class KernelSem
{
	public:

		friend class Semaphore;


		KernelSem(int init=1);
		virtual ~KernelSem  ();

		virtual int wait (Time maxTimeToWait);
		virtual int signal(int n=0);
		int val () const;
		int getIDofSem();
		SleepList::Elem* getFirstInSleepList();

		Queue* blockedOnSem;
		SleepList* sleepList;
		void increaseValue();
		void decreaseValue();

	private:

		ID id;
		static ID lastID;

		int value;


};

#endif
