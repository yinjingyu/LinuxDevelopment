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

#include "CThreadUsingMsgLoop.h"
#include "./include/CStatus.h"
#include "./include/CUsrBizUsingMsgLoop.h"
#include "./include/CMsgObserver.h"
#include "./include/CMsgLoopMgrUsingUsrDefQueue.h"
#include "./include/CThread.h"
#include "./include/CThreadInitFinishedNotifier.h"
#include <string.h>
#include <iostream>

CThreadUsingMsgLoop:: CThreadUsingMsgLoop(const char * strThreadName,CMsgObserver * pMsgObserver)
{
	if( 0 == pMsgObserver)
	{
		std::cout <<"In Construction of CThreadUsingMsgLoop,parameter is null"<<std::endl;
		throw "In Construction of CThreadUsingMsgLoop,parameter is null";
	}
	if(0 == strThreadName || 0 == strlen(strThreadName))
	{
		delete pMsgObserver;
		std::cout << "In Consruction of CThreadUsingMsgLoop ,strThreadName is bad!" << std::endl;
		throw "In Construction of CThreadForMsgLoop,paremeter is null";
	}

	m_sThreadName = strThreadName;

	m_bWaitForDeath =false;

	m_pThread = new CThread(new CUsrBizUsingMsgLoop(new CMsgLoopMgrUsingUsrDefQueue(strThreadName,pMsgObserver)));
}

CThreadUsingMsgLoop:: CThreadUsingMsgLoop(const char * strThreadName,CMsgObserver * pMsgObserver,bool bWaitForDeath)
{
	if(0 == pMsgObserver)
	{
		std::cout<< "In Construction of CThreadUsingMsgLoop ,pMsgObserver is null" << std::endl;
		throw "In Construction of CThreadUsingMsgLoop ,pMsgObserver is null";
	}
	if(0 == strThreadName || (strlen(strThreadName) ==0 ))
	{
		delete pMsgObserver; 	 	
		throw "In Construction of CThreadForMsgLoop,paremeter is null";
	}

	m_sThreadName = strThreadName;

	m_bWaitForDeath =bWaitForDeath;

	m_pThread = new CThread(new CUsrBizUsingMsgLoop(new CMsgLoopMgrUsingUsrDefQueue(strThreadName,pMsgObserver)),bWaitForDeath);
}

CThreadUsingMsgLoop:: ~CThreadUsingMsgLoop()
{
	if(m_bWaitForDeath && m_pThread != 0)
	{
		CStatus s = m_pThread->WaitForDeath();
		if(!s.IsSuccess())
		{
			std::cout << "In destructor of CThreadUsingMsgLoop,m_pThread->WaitForDeath error"<< std::endl;
			throw CStatus(-1,0);
		}
	}
}

CStatus CThreadUsingMsgLoop::Run(void * pContext)
{
	if(m_pThread == 0)
	{
		std::cout << "In CThreadUsingMsgLoop::Run ,m_pThread is null" << std::endl;
		return CStatus(-1,0);
	}

	CEvent event;
	CThreadInitFinishedNotifier notifier(&event);

	SInitialParameter para;
	para.pContext = pContext;
	para.pNotifier = &notifier;

	CStatus s_r = m_pThread->Run(&para);
	if(!s_r.IsSuccess())
	{
		m_bWaitForDeath = false;
		m_pThread = 0;
		std::cout << "In CThreadForMsgLoop::Run invoke run failed!"<< std::endl;
		return CStatus(-1,0);
	}
	
	//主线程等待子线程初始化完毕
	CStatus s_w = event.Wait();
	if(!s_w.IsSuccess())
	{
		std::cout <<"In CThreadForMsgLoop::Run , event.wait failed !"<<std::endl;
		return CStatus(-1,0);
	}
	
	//判断子线程初始化是否成功
	if(!notifier.IsInitialSuccess())
	{
		std::cout <<"In CThreadForMsgLoop::Run : Thread initial failed"<<std::endl;
		return CStatus(-1,0);
	}
	else
		return CStatus(0,0);
}

const char * CThreadUsingMsgLoop::GetThreadName()
{
	return m_sThreadName.c_str();
}
