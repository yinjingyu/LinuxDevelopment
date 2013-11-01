/*
 * =====================================================================================
 *
 *       Filename:  CUseMsgLoopWithoutThread.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月30日 16时23分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CUseMsgLoopWithoutThread.h"
#include "CStatus.h"
#include "CMsgObserver.h"
#include "CUsrBizUsingMsgLoop.h"
#include "CMsgLoopMgrUsingUsrDefQueue.h"
#include "CThreadInitFinishedNotifier.h"
#include "CThreadUsingMsgLoop.h"
#include <iostream>
#include <string.h>
CUseMsgLoopWithoutThread::CUseMsgLoopWithoutThread(const char * strThreadName, CMsgObserver * pMsgObserver)
{
	if(pMsgObserver == 0)
	{
		std::cout << "In CUseMsgLoopWithoutThread contructor,pMsgObserver is null" << std::endl;
		throw "In CUseMsgLoopWithoutThread contructor,pMsgObserver is null"; 	
	}
	if(0 == strThreadName || strlen(strThreadName) == 0 )
	{
		std::cout <<"In construction of CUseMsgLoopWithoutThread : parameter is null"<< std::endl;
		throw "error in CUseMsgLoopWithoutThread contructor!";
	}
	try
	{
		m_pUsrBiz = new CUsrBizUsingMsgLoop(new CMsgLoopMgrUsingUsrDefQueue(strThreadName,pMsgObserver));
	}
	catch(...)
	{
		std::cout <<"In CUseMsgLoopWithoutThread contructor, m_pUsrBiz initialize failed!" <<std::endl;
	}
}


CUseMsgLoopWithoutThread::~CUseMsgLoopWithoutThread()
{
	if(0 != m_pUsrBiz)
	{
		delete m_pUsrBiz;
		m_pUsrBiz = 0;
	}
}

CStatus CUseMsgLoopWithoutThread::Run(void * pContext)
{
	//由于消息循环在进入循环前一定要进行特定的初始化，且会通过事件
	//将初始化结构通知给创建进程。
	//为了兼容让面的步骤，我们这里做了一个没有事件（传0进去）的初始化
	//结果通知对象notifier，这样就很好的达到了和之前的消息循环的兼容
	CThreadInitFinishedNotifier notifier(0);
	SInitialParameter para;
	para.pContext = pContext;
	para.pNotifier = &notifier;

	return m_pUsrBiz->RunClientBusiness(&para);
}






