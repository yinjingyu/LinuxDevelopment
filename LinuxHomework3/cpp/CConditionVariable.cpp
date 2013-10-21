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

#include "../include/CConditionVariable.h"
#include "../include/CMutex.h"

CConditionVariable::CConditionVariable()
{
	int r = pthread_cond_init(&m_Cond,0);
	if(0 != r)
	{
		throw CStatus(-1,0,"in CConditionVariable of CConditionVariable : init conditionvariable failed!");
	}
}

CConditionVariable::~CConditionVariable()
{
	int r = pthread_cond_destroy(&m_Cond);
	if(0 != r)
	{
		throw CStatus(-1,0,"int CConditionVariable of CConditionVariable : destroy CConditionVariable failed");
	}
}

CStatus CConditionVariable::Wait(CMutex * pMutex)
{
	if(NULL == pMutex)
	{
		throw CStatus(-1,0,"in wait of CConditionVariable: paremeter is bad!");
	}
	
 	int r = pthread_cond_wait(&m_Cond, &(pMutex->m_Mutex));
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






















