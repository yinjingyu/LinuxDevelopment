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

#include "CThreadInitFinishedNotifier.h"
#include "CStatus.h"
#include <iostream>

CStatus CThreadInitFinishedNotifier:: NotifyInitialFinished(bool bIsSuccess)
{
	m_bIsSucess = bIsSuccess;
	
	if(0 == m_pEvent)
	{
		return CStatus(-1,0); 
	}
	
	CStatus s = m_pEvent->Set();
	if(!s.IsSuccess())
	{
		std::cout << "In CThreadInitFinishedNotifier::NotifyInitialFinished , m_pEvent->Set failed!" << std::endl;
		return CStatus(-1,0);
	}
	return CStatus(0,0);
}

bool CThreadInitFinishedNotifier::IsInitialSuccess()
{
	return m_bIsSucess;
}

CThreadInitFinishedNotifier::CThreadInitFinishedNotifier(CEvent * pEvent)
{
	m_pEvent = pEvent;
	m_bIsSucess = false;
}

CThreadInitFinishedNotifier::~CThreadInitFinishedNotifier()
{

}


