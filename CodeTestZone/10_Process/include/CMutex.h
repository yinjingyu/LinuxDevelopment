
#ifndef CMUTEX_H
#define CMUTEX_H

#include <pthread.h>
#include "CStatus.h"
class CMutex
{
public:
	CMutex();

	//通过由外部传入pthread_mutex_t来构造互斥量
	explicit CMutex(pthread_mutex_t * pMutex);
	
	CMutex(const char * pstrFileName, int nType);

	CMutex(const char * pstrFileName, pthread_mutex_t * pMutex);

	virtual ~CMutex();
	

	CStatus Lock();
	CStatus Unlock();
	
	CMutexInterface * GetMutexInterface();

private:
	CMutexInterface * m_pMutex;
};



#endif