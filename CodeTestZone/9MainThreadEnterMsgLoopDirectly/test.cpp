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
#include "IUsrBizForExecObj.h"
#include "CMessage.h"
#include "CUsrDefMsgQueue.h"
#include "CStatus.h"
#include "CMsgLoopManager.h"
#include "CMsgLoopMgrUsingUsrDefQueue.h"
#include "MessageIDTable.h"
#include "CThread.h"
#include "CUsrBizUsingMsgLoop.h"
#include "CCommunicationNameServer.h"
#include "CThreadUsingMsgLoop.h"
#include "CUseMsgLoopWithoutThread.h"

using namespace std;


class CChildObserver : public CMsgObserver
{
	public:
	
	//由CMessageLoopManager在进入消息循环前调用
	virtual CStatus Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext)
	{
		pMsgLoopMgr->Register(1,(CallBackFunctionOfMsgObserver)(&CChildObserver::On_1));
		CCommunicationNameServer::SendMessage("main",new CMessage(1));
		return CStatus(0,0);
	}

	CStatus On_1(CMessage * pMsg)
	{
		cout << "in child On_1" << endl;
 		return CStatus(QUIT_MESSAGE_LOOP,0);
	}

};

class CMainObserver : public CMsgObserver
{
	private: 

	CThreadUsingMsgLoop * m_pTChild;

	public:
	
	~CMainObserver()
	{
		delete m_pTChild;
	}

	virtual CStatus Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext)
	{
		pMsgLoopMgr->Register(1,(CallBackFunctionOfMsgObserver)(&CMainObserver::On_1));

		m_pTChild = new CThreadUsingMsgLoop("child",new CChildObserver(),true);

		m_pTChild->Run(0); 		
	}


	CStatus On_1(CMessage * pm)
	{
		cout << "in main On_1" << endl;
		CCommunicationNameServer::SendMessage("child",new CMessage(1));
		return CStatus(QUIT_MESSAGE_LOOP,0);
	}
};

int main()
{
	CUseMsgLoopWithoutThread p("main",new CMainObserver());
	
	p.Run(0);
 	return 0;
}
