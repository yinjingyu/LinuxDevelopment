
#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include <map>
#include <string>

#include "CQueueForThread.h"
#include "CAddThreadObserver.h"

#include "./include/CThreadUsingMsgLoop.h"

class CThreadPool
{

friend class CThreadPoolObserver;
public:
	CThreadPool (const unsigned int nThreadAmounts);
	virtual ~CThreadPool ();
	
	CStatus DispatchMessage(CMessage * pMsg);
private:
	
	CQueueForThread * m_pThreadQueue;	
	std::map<std::string,CThreadUsingMsgLoop *> m_ThreadTable;

	CMutex m_MutexForThreadTable;
	CMutex m_MutexForThreadQueue;

	CThreadUsingMsgLoop * m_pThreadPoolMgr;
};

#endif
