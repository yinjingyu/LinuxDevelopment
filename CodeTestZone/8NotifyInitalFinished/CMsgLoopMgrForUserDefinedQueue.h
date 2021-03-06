


#ifndef CMSGLOOPMGRFORUSERDEFINEDQUEUE_H
#define CMSGLOOPMGRFORUSERDEFINEDQUEUE_H

#include "CMessageLoopManager.h"
#include "CMessageQueueByUserDefined.h"
#include "CStatus.h"
#include "CMessage.h"
#include <string>
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
	//在使用自定义队列的消息循环类中，记录消息队列拥有线程的名称
	//该名称会注册到线程通信的名字服务中去，方便其他线程与它通信
	std::string m_strThreadName;
	public:

	CMsgLoopMgrForUserDefinedQueue(const char * strThreadName,CMessageObserver * pMsgObserver);


	virtual CMessage * WaitForMessage();
	
	virtual CStatus Initialize();
	virtual CStatus Uninitialize();
};

#endif
