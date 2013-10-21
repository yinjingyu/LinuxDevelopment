
#ifndef CMUTEX_H
#define CMUTEX_H

#include <pthread.h>
#include "CStatus.h"
class CMutex
{
	private:
	pthread_mutex_t m_Mutex;

	public:
	CMutex();
	virtual ~CMutex();

	CStatus Lock();
	CStatus Unlock();
	CStatus Trylock();
};



#endif
