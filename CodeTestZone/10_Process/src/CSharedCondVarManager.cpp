#include <string.h>
#include "CSharedConditionVariableAllocator.h"
#include "CEnterCriticalSection.h"
#include "CMutex.h"
#include "CSharedCondVarPool.h"

#define MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR "mutex_for_shared_condition_variable_allocator"

CSharedCondVarManager *CSharedCondVarManager::m_pAllocator = 0;

CSharedCondVarManager::CSharedCondVarManager()
{
	m_pImpl = new CSharedCondVariableImpl;

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	if(!((m_pImpl->Initialize()).IsSuccess()))
		throw "In CSharedCondVarManager::CSharedCondVarManager(), m_pImpl->Initialize error";
}

CSharedCondVarManager::~CSharedCondVarManager()
{
	if(!((m_pImpl->Destroy()).IsSuccess()))
	{
		delete m_pImpl;
		throw "In CSharedCondVarManager::~CSharedCondVarManager(), m_pImpl->Destroy error";
	}

	delete m_pImpl;
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
	return m_pAllocator;
}

CStaus CSharedCondVarManager::Create()
{
	if(m_pAllocator == 0)
	{
		try
		{
			m_pAllocator = new CSharedCondVarManager();
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
	if(m_pAllocator == 0)
		return CStaus(0, 0);

	CMutex mutex(MUTEX_FOR_SHARED_CONDITION_VARIABLE_ALLOCATOR, MUTEX_USE_SHARED_PTHREAD);
	CEnterCriticalSection cs(&mutex);

	try
	{
		delete m_pAllocator;
	}
	catch(const char* str)
	{
		m_pAllocator = 0;

		CLLogger::WriteLogMsg(str, 0);
		return CStaus(-1, 0);
	}

	m_pAllocator = 0;

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

	if(m_pAllocator == 0)
	{
		CLLogger::WriteLogMsg("In CSharedCondVarManager::GetSharedCond(), m_pAllocator == 0", 0);
		return 0;
	}

	return (pthread_cond_t *)m_pImpl->GetSharedObject(pstrCondName);
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

	if(m_pAllocator == 0)
	{
		CLLogger::WriteLogMsg("In CSharedCondVarManager::ReleaseSharedCond(), m_pAllocator == 0", 0);
		return CStaus(-1, 0);
	}

	return m_pImpl->ReleaseSharedObject(pstrCondName);
}