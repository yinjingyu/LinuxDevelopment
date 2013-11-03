/*
 * =====================================================================================
 *
 *       Filename:  CThreadPool.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月02日 14时51分34秒
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
#include <map>
#include <string>

#include "CThreadPool.h"
#include "CAddThreadObserver.h"
#include "CThreadPoolObserver.h"
#include "CQueueForThread.h"
#include "CQuitMessage.h"

#include "./include/CEnterCriticalSection.h"
#include "./include/CMessage.h"
#include "./include/CCommunicationNameServer.h"

CThreadPool::CThreadPool(const unsigned int nThreadAmounts)
{
	if(nThreadAmounts == 0)
 	{
		std::cout << "In CThreadPool::constructor, nThreadAmounts is 0 "<<std::endl;
		throw "In CThreadPool contructor ,error!";
	}

	 m_pThreadQueue = new CQueueForThread(nThreadAmounts);
	if(0 == m_pThreadQueue)
	{
		std::cout << "In CThreadPool::constructor,new CQueueForThread failed" << std::endl;
		throw "error";
	}
	
	char * sThreadName = new char[2];
	for (int i = 0; i < nThreadAmounts; i++) {
		/* code */
		sprintf(sThreadName,"%d",i);
 		CThreadUsingMsgLoop * pThread = new CThreadUsingMsgLoop(sThreadName,new CAddThreadObserver(sThreadName),true);
		if(0 == pThread)
		{
			std::cout << "In CThreadPool::constructor, new CThreadUsingMsgLoop failed" << std::endl;
			throw "error";
		}

		CStatus s0 = pThread->Run(0);
		if(!s0.IsSuccess())
		{
			std::cout <<"In CThreadPool::constructor,pThread->Run failed!" <<std::endl;
			throw "error!";
		}
		CStatus s1 = m_pThreadQueue->PushThread(pThread);
		if(!s1.IsSuccess())
		{
			std::cout << "In CThreadPool::Constructor,m_pThreadQueue->PushThread failed!"<<std::endl;
			throw "error";
		}
		m_ThreadTable[sThreadName] = pThread;
	}

	m_pThreadPoolMgr = new CThreadUsingMsgLoop("ThreadPool",new CThreadPoolObserver(this),true);
	if(m_pThreadPoolMgr == 0)
	{
		std::cout<<"In CThreadPool::constructor,m_pThreadPoolMgr is 0"<<std::endl;
		throw "error";
	}
 
	m_pThreadPoolMgr->Run(0);
}

CThreadPool::~CThreadPool()
{
	if(m_pThreadPoolMgr != 0)
	{
		CCommunicationNameServer::SendMessage("ThreadPool",new CQuitMessage());
		delete m_pThreadPoolMgr;
		m_pThreadPoolMgr = 0;
	}
	//遍历线程池中的所有线程，并注销他们	
	std::map<std::string,CThreadUsingMsgLoop *>::iterator it;
	for(it = m_ThreadTable.begin();it != m_ThreadTable.end(); it++)
	{
		if(it->second != 0)
		{
			CStatus s = CCommunicationNameServer::SendMessage(it->second->GetThreadName(),new CQuitMessage());
			if(!s.IsSuccess())
			{
				std::cout <<"In CThreadPool::deconsructor,CCommunicationNameServer::SendMessage failed!" <<std::endl;
 			}
 			delete it->second;
			it->second =0;
		}
	}
	//删除空闲线程队列
	if(m_pThreadQueue != 0)
	{
		delete m_pThreadQueue;
		m_pThreadQueue = 0;
	}
}

CStatus CThreadPool::DispatchMessage(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		std::cout<<"In CThreadPool::DispatchMessage,pMsg is 0"<<std::endl;
		return CStatus(-1,0);
	}

	CThreadUsingMsgLoop * pThread;
	{
  		//队列是共享资源，所以必须互斥的读写队列
		CEnterCriticalSection(*m_MutexForThreadQueue);

		pThread = m_pThreadQueue->GetThread();
		if(0 == pThread)
		{
			std::cout << "In CThreadPool::DispatchMessage,pThread is 0" <<std::endl;
			return CStatus(-1,0);
		}
	}
	CStatus s1 = CCommunicationNameServer::SendMessage(pThread->GetThreadName(),pMsg);
	if (!s1.IsSuccess())
	{
		std::cout << "In CThreadPool::DispatchMessage , CCommunicationNameServer::SendMessage failed!"<<std::endl;
		return CStatus(-1,0);
			/* code */
	}
	
	return CStatus(1,0);
}



