/*
 * =====================================================================================
 *
 *       Filename:  CThreadForMsgLoop.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月30日 10时06分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CThreadForMsgLoop.h"
#include "CStatus.h"
#include "CClientBizUsingMsgLoop.h"
#include "CMessageObserver.h"
#include "CMsgLoopMgrForUserDefinedQueue.h"
#include "CThread.h"
#include "CThreadInitialFinishedNotifier.h"

CThreadForMsgLoop:: CThreadForMsgLoop(const char * strThreadName,CMessageObserver * pMsgObserver)
{
	if(0 == strThreadName || 0 == pMsgObserver)
	{
		throw CStatus(-1,0,"In Construction of CThreadForMsgLoop,paremeter is null");
	}

	m_bWaitForDeath =false;

	m_pThread = new CThread(new CClientBizUsingMsgLoop(new CMsgLoopMgrForUserDefinedQueue(strThreadName,pMsgObserver)));
}

CThreadForMsgLoop:: CThreadForMsgLoop(const char * strThreadName,CMessageObserver * pMsgObserver,bool bWaitForDeath)
{
	if(0 == strThreadName || 0 == pMsgObserver)
	{
		throw CStatus(-1,0,"In Construction of CThreadForMsgLoop,paremeter is null");
	}

	m_bWaitForDeath =bWaitForDeath;

	m_pThread = new CThread(new CClientBizUsingMsgLoop(new CMsgLoopMgrForUserDefinedQueue(strThreadName,pMsgObserver)),bWaitForDeath);
}

CThreadForMsgLoop:: ~CThreadForMsgLoop()
{
	if(m_bWaitForDeath && m_pThread != 0)
	{
		CStatus s = m_pThread->WaitForDeath();
		if(!s.IsSuccess())
		{
			throw s;
		}
	}
}

CStatus CThreadForMsgLoop::Run(void * pContext)
{
	if(m_pThread == 0)
	{
		return CStatus(-1,0,"In CThreadForMsgLoop::Run : m_pThread is null");
	}

	CEvent event;
	CThreadInitialFinishedNotifier notifier(&event);

	SInitialParameter para;
	para.pContext = pContext;
	para.pNotifier = &notifier;

	CStatus s_r = m_pThread->Run(&para);
	if(!s_r.IsSuccess())
	{
		m_bWaitForDeath = false;
		m_pThread = 0;
		return CStatus(-1,0,"In CThreadForMsgLoop::Run invoke run failed!");
	}
	
	//主线程等待子线程初始化完毕
	CStatus s_w = event.Wait();
	if(!s_w.IsSuccess())
		return s_w;
	
	//判断子线程初始化是否成功
	if(!notifier.IsInitialSuccess())
		return CStatus(-1,0,"In CThreadForMsgLoop::Run : Thread initial failed");
	else
		return CStatus(0,0);
}


