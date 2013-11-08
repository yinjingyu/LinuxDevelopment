/*
 * =====================================================================================
 *
 *       Filename:  CConditionVariable.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月21日 12时14分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CConditionVariable.h"
#include "CMutex.h"
#include "IMutexUsingPThread.h"
#include "CStatus.h"

#include <string.h>
#include <iostream>

using namespace std;

CConditionVariable::CConditionVariable()
{
	m_pConditionVariable = new pthread_cond_t;
	m_bNeedDestroy = true;
	int r = pthread_cond_init(m_pConditionVariable,0);
	if(0 != r)
	{
		throw CStatus(-1,0,"in CConditionVariable of CConditionVariable : init conditionvariable failed!");
	}
}

CConditionVariable::CConditionVariable(pthread_cond_t * pCond)
{
	if(pCond == 0)
	{
		cout << "In CConditionVariable::constructor,pCond is 0" << endl;
		throw "";
	}

	m_pConditionVariable = pCond;
	m_bNeedDestroy = false;
}

CConditionVariable::CConditionVariable(const char * pstrCondName)
{
	if(pstrCondName == 0 || strlen(pstrCondName) == 0)
	
		cout << "In CConditionVariable::Contrcutor,pstrCondName is 0"<<endl;
		throw "";
	}

	m_strCondName = pstrCondName;
	m_bNeedDestroy = false;
	m_pConditionVariable = CSharedCondVarManager::Get(pstrCondName);
}

CConditionVariable::~CConditionVariable()
{
	if(m_bNeedDestroy)
	{
		int r = pthread_cond_destroy(m_pConditionVariable);
	 	if(0 != r)
	 	{
	 		throw CStatus(-1,0,"int CConditionVariable of CConditionVariable : destroy CConditionVariable failed");
	 	}
	}

	CStatus s = CSharedCondVarManager::Release(m_strCondName.c_str());
	if(!s.IsSuccess())
	{
		cout << "In CConditionVariable::~CConditionVariable, CSharedCondVarManager::Release failed" << endl;
		throw "";
	}
}

CStatus CConditionVariable::Wait(CMutex * pMutex)
{
	if(NULL == pMutex)
	{
		return CStatus(-1,0,"in wait of CConditionVariable: paremeter is bad!");
	}
	
	CMutexInterface * pInterface = pMutex->GetMutexInterface();
	IMutexUsingPThread * p = dynamic_cast<IMutexUsingPThread *>pInterface;
	
	if( p == 0 )
	{
		cout << "In CConditionVariable::Wait , pMutex can not be changed to IMutexUsingPThread" <<endl;
		return CStatus(-1,0);
	}

	
	int r= pthread_cond_wait(m_pConditionVariable,p->GetMutexPointer());
	if( 0 !=  r)
	{
		return CStatus(-1,0,"in wait of CConditionVariable : wait failed");
	}

	return CStatus(0,0);
}

CStatus CConditionVariable::Wakeup()
{
	int r = pthread_cond_signal(&m_Cond);
	if(0 != r)
	{
		return CStatus(-1,0,"in Wakeup of CConditionVariable : wait failed");
	}
	return CStatus(0,0);
}

CStatus CConditionVariable::WakeupAll()
{
	int r = pthread_cond_broadcast(&m_Cond);
	if(0 != r)
	{
		return CStatus(-1,0,"in Wakeup of CConditionVariable : wakeup failed");
	}

	return CStatus(0,0);
}

