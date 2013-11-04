/*
 * =====================================================================================
 *
 *       Filename:  CUsrThreadObserver.cpp
 *
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年11月04日 11时24分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CUsrThreadObserver.h"
#include "MessageIDTable.h"
#include "CAddResultMessage.h"
#include "CThreadPool.h"
#include "CAddMessage.h"

#include <iostream>

CUsrThreadObserver::CUsrThreadObserver(void * pContext)
{
	m_pContext = pContext;
}

CStatus CUsrThreadObserver::Initialize(CMsgLoopManager * pLoopMgr,void * pContext)
{
	pLoopMgr->Register(QUIT_MESSAGE_LOOP,(CallBackFunctionOfMsgObserver)(&CUsrThreadObserver::On_Quit)); 
	pLoopMgr->Register(USR_REQUESTRESULT_MESSAGE,(CallBackFunctionOfMsgObserver)(&CUsrThreadObserver::On_RequestResult));

	if(m_pContext == 0)
	{
		std::cout <<"In CUsrThreadObserver::Initialize ,m_pContext is 0" << std::endl;
		return CStatus(-1,0); 	 	
	}
	
	CThreadPool * pPool = (CThreadPool *)m_pContext;
	
	for (int i = 0; i < 10; i++) {
		CStatus s = pPool->DispatchMessage(new CAddMessage(0,i));	
		if(!s.IsSuccess())
		{
			std::cout<<"In CAddThreadObserver::Initialize,pPool->DispatchMessage() failed!,already dispatched msgs: "<< i+1 <<std::endl;
			
			return CStatus(-1,0);
		}
	}
	
	return CStatus(0,0);
}

CStatus CUsrThreadObserver::On_RequestResult(CMessage * pMsg)

{
	if(pMsg == 0)
	{
		std::cout <<"In CUsrThreadObserver::On_RequestResult,pMsg is 0" << std::endl; 
	}
	
	CAddResultMessage * pResultMsg = (CAddResultMessage *)pMsg;
	std::cout << "In Usr,the received result is :" << pResultMsg->m_ciResult << std::endl;

	return CStatus(0,0);
}


CStatus CUsrThreadObserver::On_Quit(CMessage * pMsg)
{
	return CStatus(QUIT_MESSAGE_LOOP,0); 
}
