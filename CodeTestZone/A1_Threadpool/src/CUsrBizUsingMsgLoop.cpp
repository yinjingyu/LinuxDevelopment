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
#include <iostream>

CUsrBizUsingMsgLoop::CUsrBizUsingMsgLoop(CMsgLoopManager *pMsgLoopManager)
{
	if(0 == pMsgLoopManager)
	{
		std::cout <<"In CUsrBizUsingMsgLoop::Construction pMsgLoopManager is null"<<std::endl;
		throw CStatus(-1,0);
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
