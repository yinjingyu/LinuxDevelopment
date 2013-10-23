/*
 * =====================================================================================
 *
 *       Filename:  CMsgLoopMgrForUserDefinedQueue.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 16时16分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CMsgLoopMgrForUserDefinedQueue.h"
#include "CStatus.h"
#include "CMessage.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CMsgLoopMgrForUserDefinedQueue
 *  Description: 初始化消息循环类中的消息队列
 * =====================================================================================
 */
CMsgLoopMgrForUserDefinedQueue::CMsgLoopMgrForUserDefinedQueue(CMessageQueueByUserDefined * pQueue)
{
 	if(NULL == pQueue)
	{
		throw CStatus(-1,0,"in CMsgLoopMgrForUserDefinedQueue's construction function:paremeter pQueue are NULL");
	}
	m_pQueue = pQueue;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CMsgLoopMgrForUserDefinedQueue
 *  Description:  阻塞的从用户自定义的队列中取出一条消息
 * =====================================================================================
 */
CMessage * CMsgLoopMgrForUserDefinedQueue:: WaitForMessage()
{
	return m_pQueue->GetMessage();
}

CStatus CMsgLoopMgrForUserDefinedQueue::Initialize()
{
	return CStatus(0,0);
}

CStatus CMsgLoopMgrForUserDefinedQueue:: Uninitialize()
{
	return CStatus(0,0);
}
