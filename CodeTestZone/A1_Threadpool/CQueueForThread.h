
#ifndef CQUEUEFORTHREAD_H
#define CQUEUEFORTHREAD_H

#include "./include/CStatus.h"
#include "./include/CMutex.h"
#include "CThreadUsingMsgLoop.h"
#include "./include/CEvent.h"

class CQueueForThread
{
	private:
	CThreadUsingMsgLoop ** m_pQueueSpace;

	int m_iQueueTail;
	int m_iQueueHead;

	int m_iTotalRoom;
	
	CMutex m_Mutex;
	CEvent m_Event;
	private:
	CStatus EnlargeQueue();

	CStatus Push(CThreadUsingMsgLoop * pThread);
	CThreadUsingMsgLoop * Pop();

	public:
 	bool IsFull();
	bool IsEmpty();

	CQueueForThread();
	virtual ~CQueueForThread();
	
	CStatus PushThread(CThreadUsingMsgLoop * pThread);
	CThreadUsingMsgLoop * GetThread();
};

#endif
