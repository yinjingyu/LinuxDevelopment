/*
 * =====================================================================================
 *
 *       Filename:  CThreadInitialFinishedNotifier.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月30日 09时07分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CThreadInitialFinishedNotifier.h"
#include "CStatus.h"

CStatus CThreadInitialFinishedNotifier:: NotifyInitialFinished(bool bIsSuccess)
{
	m_bIsSucess = bIsSuccess;
	
	if(0 == m_pEvent)
	{
		return CStatus(-1,0,"In NotifyInitialFinished of CThreadInitialFinishedNotifier : m_pEvent is NULL"); 
	}

	return m_pEvent->Set();
}

bool CThreadInitialFinishedNotifier::IsInitialSuccess()
{
	return m_bIsSucess;
}

CThreadInitialFinishedNotifier::CThreadInitialFinishedNotifier(CEvent * pEvent)
{
	m_pEvent = pEvent;
	m_bIsSucess = false;
}

CThreadInitialFinishedNotifier::~CThreadInitialFinishedNotifier()
{

}


