/*
 * =====================================================================================
 *
 *       Filename:  CClientBusinessUsingMsgLoop.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月28日 21时11分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CClientBizUsingMsgLoop.h"
#include "CStatus.h"

CClientBizUsingMsgLoop::CClientBizUsingMsgLoop(CMessageLoopManager *pMsgLoopManager)
{
	m_pMsgLoopManager = pMsgLoopManager;
}

CClientBizUsingMsgLoop:: ~CClientBizUsingMsgLoop()
{
	delete m_pMsgLoopManager;
}

CStatus CClientBizUsingMsgLoop::RunClientBusiness(void *pContext)
{
	CStatus s = m_pMsgLoopManager->EnterMessageLoop(pContext);
	return s;
}
