/*
 * =====================================================================================
 *
 *       Filename:  CCommunicationByMessageQueue.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月28日 22时46分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CCommunicationByMsgQueue.h"
#include <iostream>
CCommunicationByMsgQueue:: CCommunicationByMsgQueue(CUsrDefMsgQueue * pMsgQueue)
{
	if(0 == pMsgQueue)
	{
		std::cout <<"in construction of CCommunicationByMessageQueue : paremeter is null"<< std::endl;
		throw CStatus(-1,0);
	}
	m_pMsgQueue = pMsgQueue;
}
CCommunicationByMsgQueue:: ~CCommunicationByMsgQueue()
{
	//基于消息队列的线程通讯实体结束后
	//应该先释放消息队列
	if(0 != m_pMsgQueue)
	{
		delete m_pMsgQueue;
		m_pMsgQueue = 0;
	}
}
CStatus CCommunicationByMsgQueue:: PostMessage(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		std::cout <<"in PostMessage of CCommunicationByMessageQueue: paremeter is null"<<std::endl;
		return CStatus(-1,0);
	}

	return m_pMsgQueue->PushMessage(pMsg);
}
