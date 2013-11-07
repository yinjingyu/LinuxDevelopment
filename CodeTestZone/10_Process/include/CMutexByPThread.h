#ifndef CMUTEXBYPTHREAD_H 
#define CMUTEXBYPTHREAD_H

#include "CStatus.h"
#include "CMutexInterface.h"

#include <pthread.h>

class CMutexUsingPThread;

class CMutexByPThread : public CMutexUsingPThread
{
public:
	CMutexByPThread();
	explicit CMutexByPThread(pthread_mutex_t * pMutex);
	virtual ~CMutexByPThread();

	virtual CStatus Initialize();
	virtual CStatus Uninitialize();

	virtual CStatus Lock();
	virtual CStatus Unlock();

	//返回指向pthread_mutex_t的指针供外部使用
	//eg：在CConditionVariable里需要使用该指针
	virtual pthread_mutex_t * GetMutexPointer();

private:
	CMutexByPThread(const CMutexByPThread &);
	CMutexByPThread & operator=(const CMutexByPThread &);

private:

	pthread_mutex_t * m_pMutex;
	
	//pthread_mutex_t 可能由类外部传入，也可能由类内部分配
	//如果要在类内部分配，就需要进行销毁
	bool m_bNeedDestroy;

};

#endif
