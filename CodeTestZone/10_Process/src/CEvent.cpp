/*
 * =====================================================================================
 *
 *       Filename:  CEvent.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月21日 15时13分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CEvent.h"
#include "CStatus.h"
#include "CEnterCriticalSection.h"
#include "CSharedEventPool.h"
#include "CSharedEventManager.h"
#include <iostream>

using namespace std;

CEvent::CEvent()
{
	m_pEventInfo = new SEventInfo;
	m_pEventInfo->bSemaphore = 0;
	m_pEventInfo->Flag = 0;
	m_pEventInfo->Context = 0;

	m_bNeedDestroy = true;
	m_strEventName = "";
}

CEvent::CEvent(bool bSemaphore)
{
	m_pEventInfo = new SEventInfo;
	if(bSemaphore)
	{
		m_pEventInfo->bSemaphore = 1;
	}
	else
	{
		m_pEventInfo->bSemaphore = 0;
	}
	m_pEventInfo->Flag = 0;
	m_pEventInfo->Context = 0;

	m_bNeedDestroy = true;
	m_strEventName = "";
}


CEvent :: CEvent(const char * pstrEventName):m_Mutex(pstrEventName,MUTEX_USE_SHARED_PTHREAD),m_Cond(pstrEventName)
{
	m_pEventInfo = CSharedEventManager::Get(pstrEventName);
	if( 0 == m_pEventInfo)
	{
		cout << "In CEvent::contructor(3),m_pEventInfo is 0" << endl;
		throw "";
	}
 	
	m_pEventInfo->bSemaphore = 0;
	m_pEventInfo->Flag = 0;
	m_pEventInfo->Context = 0;

	m_bNeedDestroy = false;
	m_strEventName = pstrEventName;
}

CEvent :: CEvent(const char * pstrEventName, bool bSemaphore):m_Mutex(pstrEventName,MUTEX_USE_SHARED_PTHREAD),m_Cond(pstrEventName)
{
	m_pEventInfo = CSharedEventManager::Get(pstrEventName);
	if( 0 == m_pEventInfo)
	{
		cout << "In CEvent::contructor(3),m_pEventInfo is 0" << endl;
		throw "";
	}
 	
	if(bSemaphore)
	{
	 	m_pEventInfo->bSemaphore = 1;
	}
	else
	{
		m_pEventInfo->bSemaphore = 0;
	}
	m_pEventInfo->Flag = 0;
	m_pEventInfo->Context = 0;

	m_bNeedDestroy = false;
	m_strEventName = pstrEventName;
}

CEvent::~CEvent()
{
	if(m_bNeedDestroy)
	{
		delete m_pEventInfo;
	}
	else
	{
		CStatus s = CSharedEventManager::Release(m_strEventName.c_str());
		if(!s.IsSuccess())
		{
			cout << "In CEvent::deconstructor, CSharedEventManager::Release failed!" << endl;
			throw "";
		}
	}
}

CStatus CEvent::Set()
{
	{
		CEnterCriticalSection cs(&m_Mutex);
		m_pEventInfo->Flag++;
	}

	CStatus s = m_Cond.Wakeup();

	if(!s.IsSuccess())
	{
		return CStatus(-1,0,"in set of CEvent : wakeup failed");
	}
	else
	{
		return CStatus(0,0);
	}
}

CStatus CEvent::Wait()
{
	{
		CEnterCriticalSection cs(&m_Mutex);
		while(m_pEventInfo->Flag == 0)
		{
			CStatus s = m_Cond.Wait(&m_Mutex);
			if(!s.IsSuccess())
				return CStatus(-1,0,"in Wait of CEvent : wait failed!");
		}
		if(m_pEventInfo->bSemaphore != 0)
			m_pEventInfo->Flag--;
		else
			m_pEventInfo->Flag = 0;
	}

	return CStatus(0,0);
}




















