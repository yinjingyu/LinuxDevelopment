#include <string.h>
#include "CSharedEventManager.h"
#include "CEnterCriticalSection.h"
#include "CMutex.h"
#include "CSharedEventPool.h"

#include <iostream>
using namespace std;

#define MUTEX_FOR_SHARED_EVENT_ALLOCATOR "mutex_for_shared_event_allocator"

CSharedEventManager *CSharedEventManager::m_pShEventMgr = 0;

CSharedEventManager::CSharedEventManager()
{
	m_pShEventPool = new CSharedEventPool;

	CMutex mutex(MUTEX_FOR_SHARED_EVENT_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(!((m_pShEventPool->Initialize()).IsSuccess()))
	{
		cout << "In CSharedEventManager::CSharedEventManager(), m_pShEventPool->Initialize error" << endl;
		throw "In CSharedEventManager::CSharedEventManager(), m_pShEventPool->Initialize error";
	}
}

CSharedEventManager::~CSharedEventManager()
{
	if(!((m_pShEventPool->Destroy()).IsSuccess()))
	{
		cout << "In CSharedEventManager::~CSharedEventManager(), m_pShEventPool->Destroy error"<<endl;
		delete m_pShEventPool;
		throw "In CSharedEventManager::~CSharedEventManager(), m_pShEventPool->Destroy error";
	}

	delete m_pShEventPool;
}

SEventInfo* CSharedEventManager::Get(const char *pstrEventName)
{
	CSharedEventManager *p = CSharedEventManager::GetInstance();
	if(p == 0)
		return 0;

	return p->GetASharedEventInfo(pstrEventName);
}

CStatus CSharedEventManager::Release(const char *pstrEventName)
{
	CSharedEventManager *p = CSharedEventManager::GetInstance();
	if(p == 0)
		return CStatus(-1, 0);

	return p->ReleaseAEventInfo(pstrEventName);
}

CSharedEventManager* CSharedEventManager::GetInstance()
{
	return m_pShEventMgr;
}

CStatus CSharedEventManager::Create()
{
	if(m_pShEventMgr == 0)
	{
		try
		{
			m_pShEventMgr = new CSharedEventManager();
		}
		catch(const char *str)
		{
			cout << "In CSharedEventManager::Create, new CSharedEventManager faield!" << endl;
			return CStatus(-1, 0);
		}
	}

	return CStatus(0, 0);
}

CStatus CSharedEventManager::Destroy()
{
	if(m_pShEventMgr == 0)
		return CStatus(0, 0);

	CMutex mutex(MUTEX_FOR_SHARED_EVENT_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	try
	{
		delete m_pShEventMgr;
	}
	catch(const char* str)
	{
		m_pShEventMgr = 0;

		cout << "In CSharedEventManager::Destroy, delete m_pShEventMgr failed!" << endl;
		return CStatus(-1, 0);
	}

	m_pShEventMgr = 0;

	return CStatus(0, 0);
}

SEventInfo *CSharedEventManager::GetASharedEventInfo(const char *pstrEventName)
{
	if(pstrEventName == 0)
		return 0;

	int len = strlen(pstrEventName);
	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))
		return 0;

	CMutex mutex(MUTEX_FOR_SHARED_EVENT_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(m_pShEventMgr == 0)
	{
		cout << "In CSharedEventManager::GetSharedEvent(), m_pShEventMgr == 0" << endl;
		return 0;
	}

	return (SEventInfo *)m_pShEventPool->GetASharedObjetc(pstrEventName);
}

CStatus CSharedEventManager::ReleaseAEventInfo(const char *pstrEventName)
{
	if(pstrEventName == 0)
		return CStatus(-1, 0);

	int len = strlen(pstrEventName);
	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))
		return CStatus(-1, 0);

	CMutex mutex(MUTEX_FOR_SHARED_EVENT_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(m_pShEventMgr == 0)
	{
		cout << "In CSharedEventManager::ReleaseSharedEvent(), m_pShEventMgr == 0" << endl;
		return CStatus(-1, 0);
	}

	return m_pShEventPool->ReleaseASharedObject(pstrEventName);
}
