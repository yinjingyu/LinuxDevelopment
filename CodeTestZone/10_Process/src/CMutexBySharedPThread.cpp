#include <string.h>
#include "CMutexBySharedPThread.h"
#include "CLSharedMutexAllocator.h"
#include "CLLogger.h"

#include <iostream>

using namespace std;

CMutexBySharedPThread::CMutexBySharedPThread(const char *pstrMutexName)
{
	if((pstrMutexName == 0) || (strlen(pstrMutexName) == 0))
		throw "In CMutexBySharedPThread::CLMutexBySharedPThread(), pstrMutexName error";

	m_strMutexName = pstrMutexName;

	m_pMutex = CSharedMutexPoolManager::Get(pstrMutexName);
	if(m_pMutex == 0)
	{
		cout << "In CMutexBySharedPThread::CLMutexBySharedPThread(), Get error" << endl;
		throw "In CMutexBySharedPThread::CLMutexBySharedPThread(), Get error";
	}
}

CMutexBySharedPThread::~CLMutexBySharedPThread()
{
	if(!CLSharedMutexAllocator::Release(m_strMutexName.c_str()).IsSuccess())
	{
		cout << "In CMutexBySharedPThread::~CLMutexBySharedPThread(), Release error"<<endl;
		throw "In CMutexBySharedPThread::~CLMutexBySharedPThread(), Release error";
	}
}

CStatus CMutexBySharedPThread::Initialize()
{
	return CStatus(0, 0);
}

CStatus CMutexBySharedPThread::Uninitialize()
{
	return CStatus(0, 0);
}

CStatus CMutexBySharedPThread::Lock()
{
	int r = pthread_mutex_lock(m_pMutex);
	if(r != 0)
	{
		cout<<"In CMutexBySharedPThread::Lock(), pthread_mutex_lock error"<<endl;
		return CStatus(-1, 0);
	}

	return CStatus(0, 0);
}

CStatus CMutexBySharedPThread::Unlock()
{
	int r = pthread_mutex_unlock(m_pMutex);
	if(r != 0)
	{
		cout << "In CMutexBySharedPThread::Unlock(), pthread_mutex_unlock error"<<endl;
		return CStatus(-1, 0);
	}

	return CStatus(0, 0);
}

pthread_mutex_t *CMutexBySharedPThread::GetMutexPointer()
{
	return m_pMutex;
}
