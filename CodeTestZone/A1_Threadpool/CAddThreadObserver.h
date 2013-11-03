

#ifndef CADDTHREADOBSERVER_H
#define CADDTHREADOBSERVER_H

#include "./include/CMsgLoopManager.h"
#include "./include/CStatus.h"
#include "./include/CMsgObserver.h"
#include "./include/CMessage.h"
#include "CThreadUsingMsgLoop.h"
#include <iostream>

class CAddThreadObserver : public CMsgObserver
{
	public:
	
	//由CMessageLoopManager在进入消息循环前调用
	virtual CStatus Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext);
	CAddThreadObserver(const char * pStrThreadName);	

	char * m_pStrThreadName;

	private:
	
	CStatus On_Quit();
	CStatus On_Add(CMessage * pMsg);
};

#endif
