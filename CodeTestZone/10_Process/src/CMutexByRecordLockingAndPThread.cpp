/*
 * =====================================================================================
 *
 *       Filename:  CMutexByRecordLockingAndPThread.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 03时55分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <pthread.h>

#include "CMutexByRecordLockingAndPThread.h"
#include "CStatus.h"

using namespace std;

//让类内部自己new pthread_mutex_t
CMutexByRecordLockingAndPThread :: CMutexByRecordLockingAndPThread(const char * pstrFileName) :
	m_ProcessMutex(pstrFileName, MUTEX_USE_RECORD_LOCK)
{
}

//使用外部提供的pthread_mutex_t变量来构造互斥量
CMutexByRecordLockingAndPThread :: CMutexByRecordLockingAndPThread(const char * pstrFileName, pthread_mutex_t * pMutex) :
	m_ThreadMutex(pMutex), m_ProcessMutex(pstrFileName, MUTEX_USE_RECORD_LOCK)
{
}

CMutexByRecordLockingAndPThread :: ~CMutexByRecordLockingAndPThread()
{
}

CStatus CMutexByRecordLockingAndPThread :: Initialize()
{
	return CStatus(0,0);
}

CStatus CMutexByRecordLockingAndPThread :: Uninitialize()
{
	return CStatus(0,0);
}

CStatus CMutexByRecordLockingAndPThread :: Lock()
{
	if( !m_ThreadMutex.Lock().IsSuccess() )
	{
		cout << "In CMutexByRecordLockingAndPThread::Lock, m_ThreadMutex.lock failed!" << endl;
		return CStatus(-1,0);
	}

	if( !m_ProcessMutex.Lock().IsSuccess() )
	{
		cout << "In CMutexByRecordLockingAndPThread::Lock, m_ProcessMutex->lock failed!" << endl;
		return CStatus(-1,0);
	}

	return CStatus(0,0);
}

CStatus CMutexByRecordLockingAndPThread :: Unlock()
{
	if( !m_ProcessMutex.Unlock().IsSuccess() )
	{
		cout << "In CMutexByRecordLockingAndPThread::Unlock , m_ProcessMutex.Unlock failed!" << endl;
		return CStatus(-1,0);
	}

	if( !m_ThreadMutex.Unlock().IsSuccess() )
	{
		cout<<"In CMutexByRecordLockingAndPThread::Unlock,m_ThreadMutex.Unlock failed!"<<endl;
		return CStatus(-1,0);
	}

	return CStatus(0,0);
}



















