/*
 * =====================================================================================
 *
 *       Filename:  CAddThreadObserver.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月02日 22时17分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CAddThreadObserver.h"
#include "MessageIDTable.h"
#include "CAddMessage.h"
#include "CAddThreadJobDoneMessage.h"

#include "./include/CMsgLoopManager.h"
#include "./include/CMessage.h"
#include "./include/CCommunicationNameServer.h"
#include "./include/CStatus.h"

#include <iostream>
#include <string.h>
CAddThreadObserver::CAddThreadObserver(const char * pStrThreadName)
{
	if(pStrThreadName == 0)
	{
		std::cout << "in CAddThreadObserver::constructor,pStrThreadName is 0" <<std::endl;
		throw "error";
 	}
	
	m_pStrThreadName = new char[strlen(pStrThreadName)+1];
	strcpy(m_pStrThreadName,pStrThreadName);
}

CStatus CAddThreadObserver::Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext)
{
	pMsgLoopMgr->Register(ADD_MESSAGE,(CallBackFunctionOfMsgObserver)(&CAddThreadObserver::On_Add));
	pMsgLoopMgr->Register(QUIT_MESSAGE_LOOP,(CallBackFunctionOfMsgObserver)(&CAddThreadObserver::On_Quit));
	return CStatus(0,0);
 }

CStatus CAddThreadObserver::On_Add(CMessage * pMsg)
{
	CAddMessage * pAddMsg = (CAddMessage *)pMsg;
	std::cout << "add result is :" << pAddMsg->m_cOpt1 + pAddMsg->m_cOpt2 << std::endl;
	
	CStatus s = CCommunicationNameServer::SendMessage("ThreadPool",new CAddThreadJobDoneMessage(m_pStrThreadName));
	if(!s.IsSuccess())
	{
		std::cout<<"In CAddThreadObserver::On_Add , CCommunicationNameServer::SendMessage failed!"<<std::endl;
		return CStatus(QUIT_MESSAGE_LOOP,0);
	}

 	return CStatus(0,0);
}

CStatus CAddThreadObserver::On_Quit()
{
	return CStatus(QUIT_MESSAGE_LOOP,0);
}
