/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
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

class CAdder : public CClientBusinessForExecObj  
{
	public:
	CAdder(CMessageLoopManager * pMsgLoopMgr)
	{
		m_pMsgLoopMgr = pMsgLoopMgr;
	}
	virtual CStatus RunClientBusiness(void * pContext)
	{
		 return m_pMsgLoopMgr->EnterMessageLoop(pContext);	
	}

	private :
	CMessageLoopManager * m_pMsgLoopMgr;
};


/*
 * =====================================================================================
 *        Class:  CMyMsgProcessor
 *  Description:  消息循环机制中具体的分发消息的实现类
 * =====================================================================================
 */
class CMyMsgProcessor : public CMsgLoopMgrForUserDefinedQueue
{
	public:
	
	CMyMsgProcessor(CMessageQueueByUserDefined * pQueue):CMsgLoopMgrForUserDefinedQueue(pQueue)
	{
	}

	virtual CStatus DispatchMessage(CMessage * pMsg)
	{
		if(NULL == pMsg)
		{
			return CStatus(-1,0,"In DispatchMessage of CMyMsgProcessor : paremeter pMsg is NULL");
		}

		switch(pMsg->m_clMsgID)
		{
			case ADD_MESSAGE:
				{
				CAddMessage * pAddMessage = (CAddMessage *)pMsg;
				cout << pAddMessage->m_op1 + pAddMessage->m_op2 << endl;
				break;
				}
			case QUIT_MESSAGE_LOOP:
				return CStatus(QUIT_MESSAGE_LOOP,0);
		}
		return CStatus(0,0);
	}
};


int main()
{
	CMessageQueueByUserDefined * pQueue = new CMessageQueueByUserDefined();

	CMessageLoopManager * pMsgLoopMgr = new CMyMsgProcessor(pQueue);
	
	//CThread 需要传入一个业务逻辑实现类
	//CAdder是业务实现类，
	//它的业务就是等待别人给它发送加法工作业务，如果没有人发，他就阻塞
	//如果别人给它发了加法业务，它根据发来的消息执行相应操作来处理消息
	//这整套业务正好符合了消息循环机制
	//所以我们传给它一个消息循环机制的封装类,让这个消息循环的封装来特它实现业务
	//消息循环机制的实现类 CMyMsgProcessor
	//所以这个业务逻辑线程就是消息队列的拥有者和读取者，其他的所有线程都是消息队列的
	//发送消息者
	CThread * t = new CThread(new CAdder(pMsgLoopMgr));

	t->Run(0);

	{
		cout << "main thread is going to sent the 1+1 msg" << endl;
		CAddMessage * pAddMsg = new CAddMessage(1,1);
		pQueue->PushMessage(pAddMsg);
		sleep(1);
	}

	cout << "main thread stop sendng message for 5s" << endl;
	sleep(5);

	{
		cout << "main thread is going to sent the 1+2 msg" << endl;
		CAddMessage * pAddMsg = new CAddMessage(1,2);
		pQueue->PushMessage(pAddMsg);
		sleep(2);
	}

	cout << "main thread stop sendng message for 5s" << endl;
	sleep(5);

	{
		cout << "main thread is going to sent the end msg" << endl;
		CQuitMessage * pQuitMsg = new CQuitMessage();
		pQueue->PushMessage(pQuitMsg);
		sleep(2);
	}

	t->WaitForDeath();
 	return 0;
}
