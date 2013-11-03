/*
 * =====================================================================================
 *
 *       Filename:  CMessageQueueByUserDefined.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 20时23分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
  *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "./include/CEnterCriticalSection.h"
#include "CQueueForThread.h"
#include "./include/CStatus.h"
#include "./include/CEvent.h"
#include "CThreadUsingMsgLoop.h"
#include <iostream>

#define QUEUE_AUTO_INCREMENT_SIZE 10
#define QUEUE_INITIAL_SIZE 20

CQueueForThread::CQueueForThread()
{
	m_pQueueSpace = new CThreadUsingMsgLoop * [QUEUE_INITIAL_SIZE];
	m_iQueueTail = 0;
	m_iQueueHead = m_iQueueTail;

	m_iTotalRoom = QUEUE_INITIAL_SIZE;

	//设置事件作为记录型条件变量来使用	
	m_Event.UseAsRecordCondVar(true);
}

 CQueueForThread::CQueueForThread(const unsigned int nThreadAmounts)
{
	m_pQueueSpace = new CThreadUsingMsgLoop * [nThreadAmounts];
	m_iQueueTail = 0;
	m_iQueueHead = m_iQueueTail;

	m_iTotalRoom = nThreadAmounts;

	//设置事件作为记录型条件变量来使用	
	m_Event.UseAsRecordCondVar(true);
}

CQueueForThread::~CQueueForThread()
{
	//注销的任务交给线程池来做
//	while(true)
//	{
//		CThreadUsingMsgLoop * pThread = Pop();
//		if(0 == pThread)
//			break;
//		delete pThread;
//	}
	
	delete m_pQueueSpace;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  EnlargeQueue
 *  Description:  扩展当前的消息队列
 * =====================================================================================
 */
CStatus CQueueForThread::EnlargeQueue()
{
	CThreadUsingMsgLoop ** p = new CThreadUsingMsgLoop * [m_iTotalRoom+QUEUE_AUTO_INCREMENT_SIZE];
	
	int index = 0;

	//队列中有一个单元不用，所以当队列满的状况下共有元素 m_iTotalRoom - 1 个
	//将这m_iTotalRoom - 1个元素拷贝到新存储单元中去，
	for(int i = 0; i < m_iTotalRoom - 1; i++)
	{
		index = (m_iQueueHead+i) %  m_iTotalRoom; 
		p[i] = m_pQueueSpace[index];
	}

	//新队列从下表0开始存储，所以转存完后要校准队列头和尾的位置
	m_iQueueHead = 0;
	m_iQueueTail = m_iTotalRoom - 1;
	m_iTotalRoom += QUEUE_AUTO_INCREMENT_SIZE;

	delete m_pQueueSpace;
	
	m_pQueueSpace = p;

	return CStatus(0,0);
}

bool CQueueForThread::IsFull()
{
	//如果队列尾指针+1 == 队列头指针说明队列满
	if( (m_iQueueTail + 1) % m_iTotalRoom == m_iQueueHead)
		return true; 	 	 	
	else
		return false;
}

bool CQueueForThread::IsEmpty()
{
	if(m_iQueueHead == m_iQueueTail)
		return true;
	else
	 	return false;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Push
 *  Description:  以互斥的方式向消息队列中插入一条消息
 * =====================================================================================
 */
CStatus CQueueForThread::Push(CThreadUsingMsgLoop * pThread)
{
	CEnterCriticalSection ecs(&m_Mutex);	
	if( IsFull() )
	{
		CStatus s = EnlargeQueue();
		if(!s.IsSuccess())
		{
			std::cout <<"In CQueueForThread::Push ,failed to enlarge the queue"<<std::endl;
			return CStatus(-1,0);
		}
	}

	m_pQueueSpace[m_iQueueTail] = pThread;
	m_iQueueTail = (m_iQueueTail + 1) % m_iTotalRoom;

	return CStatus(0,0);
}
	
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Pop
 *  Description:  以互斥的方式从队列中取出一条消息
 * =====================================================================================
 */
CThreadUsingMsgLoop * CQueueForThread::Pop()
{
	CEnterCriticalSection ecs(&m_Mutex);
 	if( IsEmpty() )
 	{
 		return 0;
 	}
		
	CThreadUsingMsgLoop * pThread = m_pQueueSpace[m_iQueueHead];
 	m_iQueueHead = (m_iQueueHead + 1) % m_iTotalRoom;
	return pThread;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  PushMessage
 *  Description:  向消息队列中插入一条消息，同时使用事件来通知消息读取者新增了一条消息
 * =====================================================================================
 */
CStatus CQueueForThread::PushThread (CThreadUsingMsgLoop * pThread)
{
	if(NULL == pThread)
	{
		return CStatus(-1,0,"in PushMessage of CMessageQueueByUserDefined : pMsg is NULL");
	}
	CStatus s1 = Push(pThread);
	if( !s1.IsSuccess() )
	{
		return s1;
	}

	CStatus s2 = m_Event.Set();
	if(!s2.IsSuccess())
	{
		return s2;
	}

	return CStatus(0,0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetMessage
 *  Description:  以阻塞的方式从消息队列中取出一条消息
 * =====================================================================================
 */
CThreadUsingMsgLoop * CQueueForThread::GetThread()
{
	CStatus s = m_Event.Wait();
	if( !s.IsSuccess() )
	{
		throw s;
	}
	
	return Pop();
}



