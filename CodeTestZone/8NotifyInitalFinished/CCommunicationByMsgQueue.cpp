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

CCommunicationByMsgQueue:: CCommunicationByMsgQueue(CMessageQueueByUserDefined * pMsgQueue)
{
	if(0 == pMsgQueue)
	{
		throw CStatus(-1,0,"in construction of CCommunicationByMessageQueue : paremeter is null");
	}
	m_pMsgQueue = pMsgQueue;
}
CCommunicationByMsgQueue:: ~CCommunicationByMsgQueue()
{

}
CStatus CCommunicationByMsgQueue:: PostMessage(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		return CStatus(-1,0,"in PostMessage of CCommunicationByMessageQueue: paremeter is null");
	}

	return m_pMsgQueue->PushMessage(pMsg);
}
