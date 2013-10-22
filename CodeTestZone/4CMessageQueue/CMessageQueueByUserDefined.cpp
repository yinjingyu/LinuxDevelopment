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

#include "CMessageQueueByUserDefined.h"
#include "CStatus.h"
#include "CMessage.h"

#define QUEUE_AUTO_INCREMENT_SIZE 2
#define QUEUE_INITIAL_SIZE 3

CMessageQueueByUserDefined::CMessageQueueByUserDefined()
{
	m_pQueueSpace = new CMessage * [QUEUE_INITIAL_SIZE];
	m_iQueueTail = 0;
	m_iQueueHead = m_iQueueTail;

	m_iTotalRoom = QUEUE_INITIAL_SIZE;
}

CMessageQueueByUserDefined::~CMessageQueueByUserDefined()
{
	delete m_pQueueSpace;
}

CStatus CMessageQueueByUserDefined::EnlargeQueue()
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

bool CMessageQueueByUserDefined::IsFull()
{
	//如果队列尾指针+1 == 队列头指针说明队列满
	if( (m_iQueueTail + 1) % m_iTotalRoom == m_iQueueHead)
		return true; 	 	 	
	else
		return false;
}

bool CMessageQueueByUserDefined::IsEmpty()
{
	if(m_iQueueHead == m_iQueueTail)
		return true;
	else
	 	return false;
}

CStatus CMessageQueueByUserDefined::Push(CMessage * pMsg)
{
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
	
CMessage * CMessageQueueByUserDefined::Pop()
{
	if( IsEmpty() )
	{
		return 0;
	}
		
	CMessage * pMsg = m_pQueueSpace[m_iQueueHead];
	m_iQueueHead = (m_iQueueHead + 1) % m_iTotalRoom;
	
	return pMsg;
}
