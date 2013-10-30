/*
 * =====================================================================================
 * *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 10时57分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "CClientBusinessForExecObj.h"
#include "CMessage.h"
#include "CMessageQueueByUserDefined.h"
#include "CStatus.h"
#include "CMessageLoopManager.h"
#include "CMsgLoopMgrForUserDefinedQueue.h"
#include "MessageIDTable.h"
#include "CThread.h"
#include "CClientBizUsingMsgLoop.h"
#include "CCommunicationNameServer.h"
#include "CThreadForMsgLoop.h"

using namespace std;

class CAddMessage : public CMessage
{
	public:
	CAddMessage(int op1, int op2):CMessage(ADD_MESSAGE)
	{
		m_op1 = op1;
		m_op2 = op2;
	}

	int m_op1;
	int m_op2;
};

class CQuitMessage : public CMessage
{
	public:
	CQuitMessage():CMessage(QUIT_MESSAGE_LOOP)
	{
	}
};

class CMyMsgProcessor : public CMessageObserver
{
	public:
	
	//由CMessageLoopManager在进入消息循环前调用
	virtual CStatus Initialize(CMessageLoopManager * pMsgLoopMgr, void * pContext)
	{
		pMsgLoopMgr->Register(ADD_MESSAGE,(CallBackFunctionOfMsgObserver)(&CMyMsgProcessor::On_AddMsg));
		pMsgLoopMgr->Register(QUIT_MESSAGE_LOOP,(CallBackFunctionOfMsgObserver)(&CMyMsgProcessor::On_QuitMsg));
	
		return CStatus(0,0);
	}

	CStatus On_AddMsg(CMessage * pMsg)
	{
		CAddMessage * pAddMsg = (CAddMessage *)pMsg;
		cout << pAddMsg->m_op1 + pAddMsg->m_op2 << endl;
 		return CStatus(0,0);
	}

	CStatus On_QuitMsg(CMessage * pMsg)
	{
		return CStatus(QUIT_MESSAGE_LOOP,0);
	}

};


int main()
{
	
//	CThread * t = new CThread(new CClientBizUsingMsgLoop(new CMsgLoopMgrForUserDefinedQueue("thread_1",new CMyMsgProcessor())));
	
	//简化了使用消息循环线程
	CThreadForMsgLoop * t = new CThreadForMsgLoop("thread_1",new CMyMsgProcessor());
	t->Run(0);
 	
	{
		cout << "main thread is going to send the 1+1 msg" << endl;
	 	CCommunicationNameServer::SendMessage("thread_1",(new CAddMessage(1,1)));
		sleep(1);
	}

	cout << "main thread stop sending message for 5s" << endl;
	sleep(5);

	{
		cout << "main thread is going to send the 1+2 msg" << endl;
	 	CCommunicationNameServer::SendMessage("thread_1",(new CAddMessage(1,2)));
		sleep(2);
	}

	cout << "main thread stop sending message for 5s" << endl;
	sleep(5);

	{
		cout << "main thread is going to send the end msg" << endl;
	 	CCommunicationNameServer::SendMessage("thread_1",new CQuitMessage());
		sleep(2);
	}

 	return 0;
}
