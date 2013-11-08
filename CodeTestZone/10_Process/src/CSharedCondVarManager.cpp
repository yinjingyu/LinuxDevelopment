#include <string.h>
#include "CSharedCondVarManager.h"
#include "CEnterCriticalSection.h"
#include "CMutex.h"
#include "CSharedConditionVariablePool.h"

#include "CStaus.h"
#include <iostream>
using namespace std;
#define MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR "mutex_for_shared_condition_variable_allocator"

CSharedCondVarManager *CSharedCondVarManager::m_pShCondVarMgr = 0;

CSharedCondVarManager::CSharedCondVarManager()
{
	m_pShCondVarPool = new CSharedCondVariableImpl;

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(!((m_pShCondVarPool->Initialize()).IsSuccess()))
		throw "In CSharedCondVarManager::CSharedCondVarManager(), m_pShCondVarPool->Initialize error";
}

CSharedCondVarManager::~CSharedCondVarManager()
{
	if(!((m_pShCondVarPool->Destroy()).IsSuccess()))
	{
		delete m_pShCondVarPool;
		throw "In CSharedCondVarManager::~CSharedCondVarManager(), m_pShCondVarPool->Destroy error";
	}

	delete m_pShCondVarPool;
}

pthread_cond_t* CSharedCondVarManager::Get(const char *pstrCondName)
{
	CSharedCondVarManager *p = CSharedCondVarManager::GetInstance();
	if(p == 0)
		return 0;

	return p->GetSharedCond(pstrCondName);
}

CStaus CSharedCondVarManager::Release(const char *pstrCondName)
{
	CSharedCondVarManager *p = CSharedCondVarManager::GetInstance();
	if(p == 0)
		return CStaus(-1, 0);

	return p->ReleaseSharedCond(pstrCondName);
}

CSharedCondVarManager* CSharedCondVarManager::GetInstance()
{
	return m_pShCondVarMgr;
}

CStaus CSharedCondVarManager::Create()
{
	if(m_pShCondVarMgr == 0)
	{
		try
		{
			m_pShCondVarMgr = new CSharedCondVarManager();
		}
		catch(const char *str)
		{
			CLLogger::WriteLogMsg(str, 0);
			return CStaus(-1, 0);
		}
	}

	return CStaus(0, 0);
}

CStaus CSharedCondVarManager::Destroy()
{
	if(m_pShCondVarMgr == 0)
		return CStaus(0, 0);

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	try
	{
		delete m_pShCondVarMgr;
	}
	catch(const char* str)
	{
		m_pShCondVarMgr = 0;
		
		cout << "In CSharedCondVarManager :: Destroy, delete m_pShCondVarMgr failed!" << endl;l
		return CStaus(-1, 0);
	}

	m_pShCondVarMgr = 0;

	return CStaus(0, 0);
}

pthread_cond_t *CSharedCondVarManager::GetSharedCond(const char *pstrCondName)
{
	if(pstrCondName == 0)
		return 0;

	int len = strlen(pstrCondName);
	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))
		return 0;

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(m_pShCondVarMgr == 0)
	{
		cout << "In CSharedCondVarManager::GetSharedCond(), m_pShCondVarMgr == 0" << endl;
		return 0;
	}

	return (pthread_cond_t *)m_pShCondVarPool->GetSharedObject(pstrCondName);
}

CStaus CSharedCondVarManager::ReleaseSharedCond(const char *pstrCondName)
{
	if(pstrCondName == 0)
		return CStaus(-1, 0);

	int len = strlen(pstrCondName);
	if((len == 0) || (len >= LENGTH_OF_SHARED_OBJECT_NAME))
		return CStaus(-1, 0);

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(m_pShCondVarMgr == 0)
	{
		cout << "In CSharedCondVarManager::ReleaseSharedCond(), m_pShCondVarMgr == 0"<<endl;
		return CStaus(-1, 0);
	}

	return m_pShCondVarPool->ReleaseSharedObject(pstrCondName);
}
