/*
 * =====================================================================================
 *
 *       Filename:  CThreadPoolObserver.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月02日 18时02分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "CThreadPoolObserver.h"
#include "CThreadUsingMsgLoop.h"
#include "MessageIDTable.h"
#include "CThreadPool.h"
#include "CAddThreadJobDoneMessage.h"

#include "./include/CMessage.h"
#include "./include/CEnterCriticalSection.h"
#include "./include/CStatus.h"

#include <string.h>
#include <map>

CStatus CThreadPoolObserver::Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext)
{
	pMsgLoopMgr->Register(JOBDONE_THREADPOOL_MESSAGE,(CallBackFunctionOfMsgObserver)(&CThreadPoolObserver::On_JobDone));
	pMsgLoopMgr->Register(QUIT_MESSAGE_LOOP,(CallBackFunctionOfMsgObserver)(&CThreadPoolObserver::On_QuitLoopMessage));

	return CStatus(1,0);
}

CThreadPoolObserver::CThreadPoolObserver(CThreadPool * pThreadPool)
{
	if(0 == pThreadPool)
	{
		std::cout << "In CThreadPoolObserver::contructor,pThreadPool is 0" <<std::endl;
		throw "error!";
	}
	m_pThreadPool = pThreadPool;
}

CStatus CThreadPoolObserver::On_JobDone(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		std::cout <<"In CThreadPoolObserver::On_JobDone,pMsg is 0" <<std::endl;
		return CStatus(-1,0); 
 	}
	
	if(m_pThreadPool == 0)
	{
		std::cout << "In CThreadPoolObserver::On_JobDone, m_pThreadPool is 0" <<std::endl;
		return CStatus(-1, 0);
	}

	CAddThreadJobDoneMessage * pDoneMsg = (CAddThreadJobDoneMessage *)pMsg;
	
	//根据做完工做的线程的ID号（封装在pDoneMsg里）从线程池的map表中找到线程
	std::map<std::string,CThreadUsingMsgLoop *>::iterator it;
	{
		CEnterCriticalSection ecs(&m_pThreadPool->m_MutexForThreadTable);

		it = m_pThreadPool->m_ThreadTable.find(pDoneMsg->GetAddThreadID());

		std::cout <<"on jobdone ， thread id is"<< pDoneMsg->GetAddThreadID()<< std::endl;

		if(it == m_pThreadPool->m_ThreadTable.end())
		{
			std::cout << "In CThreadPoolObserver::On_JobDone,can't find thread!"<<std::endl;
			std::cout << "map size is " << m_pThreadPool->m_ThreadTable.size()<<std::endl;
			 return CStatus(-1,0);
		}
	}

	//将空闲线程插入到线程池的空闲队列中
	{
		CEnterCriticalSection ecs(&m_pThreadPool->m_MutexForThreadQueue);
		CStatus s = m_pThreadPool->m_pThreadQueue->PushThread(it->second);
		if(!s.IsSuccess())
		{
			std::cout << "In CThreadPoolObserver::On_JobDone,m_pThreadQueue->PushThread failed"<<std::endl; 
			return CStatus(-1,0);
		}
	}

	{
		//用来检查是否所有线程都做完了工作
		CEnterCriticalSection ecs(&m_pThreadPool->m_MutexForThreadQueue);
		if(m_pThreadPool->m_pThreadQueue->IsFull())
		{
			CStatus s = m_pThreadPool->m_WaitForQuitEvent.Set();
			if(!s.IsSuccess())
			{
				std::cout<<"In CThreadPoolObserver::On_JobDone,m_WaitForQuitEvent->Set failed!"<<std::endl;
				return CStatus(-1,0);
			}
		}
	}
	return CStatus(0,0);
}

CStatus CThreadPoolObserver::On_QuitLoopMessage(CMessage * pMsg)
{
	return CStatus(QUIT_MESSAGE_LOOP,0); 
}

