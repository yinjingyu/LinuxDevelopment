#ifndef CMutexBySharedPThread_H
#define CMutexBySharedPThread_H

#include <pthread.h>
#include <string>
#include "CMutexInterface.h"
#include "CStatus.h"
#include "IMutexUsingPThread.h"

using namespace std;

//class CMutexUsingPThread;

class CMutexBySharedPThread : public IMutexUsingPThread
{
public:
	CMutexBySharedPThread(const char *pstrMutexName);
	virtual ~CMutexBySharedPThread();

	virtual CStatus Initialize();
	virtual CStatus Uninitialize();

	virtual CStatus Lock();
	virtual CStatus Unlock();

	virtual pthread_mutex_t *GetMutexPointer();

private:
	CMutexBySharedPThread(const CMutexBySharedPThread&);
	CMutexBySharedPThread& operator=(const CMutexBySharedPThread&);

private:
	pthread_mutex_t *m_pMutex;
	string m_strMutexName;
};

#endif
