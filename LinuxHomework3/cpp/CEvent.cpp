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

#include "../include/CEvent.h"
#include "../include/CStatus.h"
#include "../include/CEnterCriticalSection.h"

CEvent::CEvent()
{
	m_iEventCounter = 0;
	m_bIsRecordCondVar = false;
}

CEvent::~CEvent()
{
}

CStatus CEvent::UseAsRecordCondVar(bool b)
{
	m_bIsRecordCondVar = b;
	return CStatus(0,0);
}

CStatus CEvent::Set()
{
	{
		CEnterCriticalSection cs(&m_Mutex);
		m_iEventCounter++;
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
		while(m_iEventCounter == 0)
		{
			CStatus s = m_Cond.Wait(&m_Mutex);
			if(!s.IsSuccess())
				return CStatus(-1,0,"in Wait of CEvent : wait failed!");
		}
		if(m_bIsRecordCondVar)
			m_iEventCounter--;
		else
			m_iEventCounter = 0;
	}

	return CStatus(0,0);
}




















