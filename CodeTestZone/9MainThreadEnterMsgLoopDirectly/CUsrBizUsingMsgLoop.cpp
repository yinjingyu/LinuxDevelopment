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
	if(0 == pMsgLoopManager)
	{
		throw CStatus(-1,0,"In CUsrBizUsingMsgLoop::Construction pMsgLoopManager is null");
	}
	m_pMsgLoopManager = pMsgLoopManager;
}

CUsrBizUsingMsgLoop:: ~CUsrBizUsingMsgLoop()
{
	if(0 != m_pMsgLoopManager)
	{
		delete m_pMsgLoopManager;
		m_pMsgLoopManager = 0;
	}
}

CStatus CUsrBizUsingMsgLoop::RunClientBusiness(void *pContext)
{
	return m_pMsgLoopManager->EnterMessageLoop(pContext);
}
