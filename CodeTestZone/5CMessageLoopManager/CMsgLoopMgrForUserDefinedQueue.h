


#ifndef CMSGLOOPMGRFORUSERDEFINEDQUEUE_H
#define CMSGLOOPMGRFORUSERDEFINEDQUEUE_H

#include "CMessageLoopManager.h"
#include "CStatus.h"
#include "CMessage.h"
#include "CMessageQueueByUserDefined.h"
/*
 * =====================================================================================
 *        Class:  CMsgLoopMgrForUserDefinedQueue
 *  Description:  消息循环机制的具体实现（CMessageLoopManager的派生类），
 *  			  使用我们用户自定义的消息队列来 接受并分发消息
 * =====================================================================================
 */
class CMsgLoopMgrForUserDefinedQueue : public CMessageLoopManager
{
	private:

	CMessageQueueByUserDefined * m_pQueue;

	public:

	CMsgLoopMgrForUserDefinedQueue(CMessageQueueByUserDefined * pQueue);
	

	virtual CMessage * WaitForMessage();
	
	virtual CStatus Initialize();
	virtual CStatus Uninitialize();
};

#endif
