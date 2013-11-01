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
#include "CEnterCriticalSection.h"
#include "CUsrDefMsgQueue.h"
#include "CStatus.h"
#include "CMessage.h"
#include "CEvent.h"

#define QUEUE_AUTO_INCREMENT_SIZE 10
#define QUEUE_INITIAL_SIZE 20

CUsrDefMsgQueue::CUsrDefMsgQueue()
{
	m_pQueueSpace = new CMessage * [QUEUE_INITIAL_SIZE];
	m_iQueueTail = 0;
	m_iQueueHead = m_iQueueTail;

	m_iTotalRoom = QUEUE_INITIAL_SIZE;

	//设置事件作为记录型条件变量来使用	
	m_Event.UseAsRecordCondVar(true);
}

CUsrDefMsgQueue::~CUsrDefMsgQueue()
{
	//如果消息队列被注销前，消息队列中还有消息，
	//就应该先注销掉所有的消息
	while(true)
	{
		CMessage * pMsg = Pop();
		if(0 == pMsg)
			break;
		delete pMsg;
	}
	
	delete m_pQueueSpace;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  EnlargeQueue
 *  Description:  扩展当前的消息队列
 * =====================================================================================
 */
CStatus CUsrDefMsgQueue::EnlargeQueue()
{
	CMessage ** p = new CMessage * [m_iTotalRoom+QUEUE_AUTO_INCREMENT_SIZE];
	
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

bool CUsrDefMsgQueue::IsFull()
{
	//如果队列尾指针+1 == 队列头指针说明队列满
	if( (m_iQueueTail + 1) % m_iTotalRoom == m_iQueueHead)
		return true; 	 	 	
	else
		return false;
}

bool CUsrDefMsgQueue::IsEmpty()
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
CStatus CUsrDefMsgQueue::Push(CMessage * pMsg)
{
	CEnterCriticalSection ecs(&m_Mutex);	
	if( IsFull() )
	{
		CStatus s = EnlargeQueue();
		if(!s.IsSuccess())
			return CStatus(-1,0,"failed to Enlarge the queue!");
	}

	m_pQueueSpace[m_iQueueTail] = pMsg;
	m_iQueueTail = (m_iQueueTail + 1) % m_iTotalRoom;

	return CStatus(0,0);
}
	
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Pop
 *  Description:  以互斥的方式从队列中取出一条消息
 * =====================================================================================
 */
CMessage * CUsrDefMsgQueue::Pop()
{
	CEnterCriticalSection ecs(&m_Mutex);
 	if( IsEmpty() )
 	{
 		return 0;
 	}
		
	CMessage * pMsg = m_pQueueSpace[m_iQueueHead];
 	m_iQueueHead = (m_iQueueHead + 1) % m_iTotalRoom;
	return pMsg;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  PushMessage
 *  Description:  向消息队列中插入一条消息，同时使用事件来通知消息读取者新增了一条消息
 * =====================================================================================
 */
CStatus CUsrDefMsgQueue::PushMessage(CMessage * pMsg)
{
	if(NULL == pMsg)
	{
		return CStatus(-1,0,"in PushMessage of CMessageQueueByUserDefined : pMsg is NULL");
	}
	CStatus s1 = Push(pMsg);
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
CMessage * CUsrDefMsgQueue::GetMessage()
{
	CStatus s = m_Event.Wait();
	if( !s.IsSuccess() )
	{
		throw s;
	}
	
	return Pop();
}



