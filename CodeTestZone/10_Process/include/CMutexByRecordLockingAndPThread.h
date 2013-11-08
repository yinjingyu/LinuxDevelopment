

#ifndef CMutexByRecordLockingAndPThread_H
#define CMutexByRecordLockingAndPThread_H

#include "CStatus.h"
#include "CMutexInterface.h"
#include "CMutex.h"

/*
 * =====================================================================================
 *        Class:  CMutexByRecordLockingAndPThread
 *  Description:  同时使用线程互斥量和进程间互斥量（记录锁）
 * =====================================================================================
 */
class CMutexByRecordLockingAndPThread : public CMutexInterface
{
public:
	CMutexByRecordLockingAndPThread(const char * pstrFileName);
	CMutexByRecordLockingAndPThread(const char * pstrFileName, pthread_mutex_t *pMutex);


	~CMutexByRecordLockingAndPThread();
	virtual CStatus Initialize();
	virtual CStatus Uninitialize();

	virtual CStatus Lock();
	virtual CStatus Unlock();

private:
	CMutexByRecordLockingAndPThread(const CMutexByRecordLockingAndPThread &);
	CMutexByRecordLockingAndPThread & operator =(const CMutexByRecordLockingAndPThread &);

private:
	CMutex m_ThreadMutex;
	CMutex m_ProcessMutex;
};

#endif
