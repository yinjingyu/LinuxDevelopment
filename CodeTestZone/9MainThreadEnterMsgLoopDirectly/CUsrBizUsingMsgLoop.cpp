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

#include "CUsrBizUsingMsgLoop.h"
#include "CStatus.h"

CUsrBizUsingMsgLoop::CUsrBizUsingMsgLoop(CMsgLoopManager *pMsgLoopManager)
{
	m_pMsgLoopManager = pMsgLoopManager;
}

CUsrBizUsingMsgLoop:: ~CUsrBizUsingMsgLoop()
{
	delete m_pMsgLoopManager;
}

CStatus CUsrBizUsingMsgLoop::RunClientBusiness(void *pContext)
{
	CStatus s = m_pMsgLoopManager->EnterMessageLoop(pContext);
	return s;
}
