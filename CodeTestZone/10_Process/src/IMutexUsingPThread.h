


#ifndef IMutexUsingPThread_H
#define IMutexUsingPThread_H

#include <pthread.h>

class CMutexInterface;

class IMutexUsingPThread : public CMutexInterface
{
public:
	
	IMutexUsingPThread();
	virtual ~IMutexUsingPThread();

	virtual pthread_mutex_t * GetMutexPointer();
};

#endif
