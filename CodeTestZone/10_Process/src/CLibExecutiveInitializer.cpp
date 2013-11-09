#include "CLibExecutiveInitializer.h"
#include "CCommunicationNameServer.h"
#include "CSharedMutexPoolManager.h"
#include "CSharedCondVarManager.h"
#include "CSharedEventManager.h"

#include <iostream>

using namespace std;

pthread_mutex_t CLibExecutiveInitializer::m_MutexForInitializer = PTHREAD_MUTEX_INITIALIZER;

bool CLibExecutiveInitializer::m_bInitialized = false;
bool CLibExecutiveInitializer::m_bDestroyed = false;

CStatus CLibExecutiveInitializer::Initialize()
{
	int r = pthread_mutex_lock(&m_MutexForInitializer);
	if(r != 0)
	{
		if((m_bInitialized) && (!m_bDestroyed))
			cout<< "In CLibExecutiveInitializer<<Initialize(), pthread_mutex_lock error"<<endl;

		return CStatus(-1, r);
	}

	if(m_bInitialized)
	{
		r = pthread_mutex_unlock(&m_MutexForInitializer);
		if((r != 0) && (!m_bDestroyed))
			cout<< "In CLibExecutiveInitializer<<Initialize(), pthread_mutex_unlock error" <<endl;

		return CStatus(-1, r);
	}

	try
	{
		CStatus s2 = CSharedMutexPoolManager::Create();
		if(!s2.IsSuccess())
		{
			cout<<"In CLibExecutiveInitializer<<Initialize(), CSharedMutexPoolManager<<Create error"<<endl;
			throw s2;
		}

		CStatus s3 = CSharedCondVarManager::Create();
		if(!s3.IsSuccess())
		{
			cout << "In CLibExecutiveInitializer<<Initialize(), CSharedCondVarManager<<Create error"<<endl;
			throw s3;
		}

		CStatus s4 = CSharedEventManager::Create();
		if(!s4.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Initialize(), CSharedEventManager<<Create error" << endl;
			throw s4;
		}

		CStatus s1 = CCommunicationNameServer::Create();
		if(!s1.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Initialize(), CCommunicationNameServer<<Create error"<<endl;
			throw s1;
		}

		m_bInitialized = true;
		throw CStatus(0, 0);
	}
	catch(CStatus& s)
	{
		r = pthread_mutex_unlock(&m_MutexForInitializer);
		if(r != 0)
			return CStatus(-1, r);

		return s;
	}
	catch(...)
	{
		r = pthread_mutex_unlock(&m_MutexForInitializer);
		if(r != 0)
			return CStatus(-1, r);

		return CStatus(-1, 0);
	}
}

CStatus CLibExecutiveInitializer::Destroy()
{
	int r = pthread_mutex_lock(&m_MutexForInitializer);
	if(r != 0)
	{
		if((m_bInitialized) && (!m_bDestroyed))
		 	cout<< "In CLibExecutiveInitializer<<Destroy(), pthread_mutex_lock error" <<endl;

		return CStatus(-1, r);
	}

	try
	{
		if((!m_bInitialized) || (m_bDestroyed))
			throw CStatus(-1, 0);

		CStatus s1 = CCommunicationNameServer::Destroy();
		if(!s1.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Destroy(), CCommunicationNameServer<<Destroy error" << endl;
			throw s1;
		}

		CStatus s4 = CSharedEventManager::Destroy();
		if(!s4.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Destroy(), CSharedEventManager<<Destroy error"<<endl;
			throw s4;
		}

		CStatus s3 = CSharedCondVarManager::Destroy();
		if(!s3.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Destroy(), CSharedCondVarManager<<Destroy error"<<endl;
			throw s3;
		}

		CStatus s2 = CSharedMutexPoolManager::Destroy();
		if(!s2.IsSuccess())
		{
			cout<< "In CLibExecutiveInitializer<<Destroy(), CSharedMutexPoolManager<<Destroy error"<<endl;
			throw s2;
		}
	
		m_bDestroyed = true;
		throw CStatus(0, 0);
	}
	catch(CStatus& s)
	{
		r = pthread_mutex_unlock(&m_MutexForInitializer);
		if(r != 0)
			return CStatus(-1, r);

		return s;
	}
	catch(...)
	{
		r = pthread_mutex_unlock(&m_MutexForInitializer);
		if(r != 0)
			return CStatus(-1, r);

		return CStatus(-1, 0);
	}
}
