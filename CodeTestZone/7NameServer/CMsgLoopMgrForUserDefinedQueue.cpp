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
#include "CCommunicationNameServer.h"
#include "CCommunicationByMsgQueue.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CMsgLoopMgrForUserDefinedQueue
 *  Description: 初始化消息循环类中的消息队列
 * =====================================================================================
 */
CMsgLoopMgrForUserDefinedQueue::CMsgLoopMgrForUserDefinedQueue(const char * strThreadName, CMessageObserver *pMsgObserver):CMessageLoopManager(pMsgObserver)
{
 	if(0 == strThreadName)
	{
		throw CStatus(-1,0,"in CMsgLoopMgrForUserDefinedQueue's construction function:paremeter strThreadName is NULL");
	}
	try
	{
	 	m_pQueue = new CMessageQueueByUserDefined();
	}
	catch(...)
	{
		throw CStatus(-1,0,"in construction of CMsgLoopMgrForUserDefinedQueue: new CMessageQueueByUserDefined failed");
	}
	
	m_strThreadName = strThreadName;
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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Initialize
 *  Description:  让线程在进入消息循环前进行特定的初始化工作
 *  		      1、获得名字服务对象，将当前线程的名字和与当前线程通信的
 *  		      对外暴露的通信对象（CCommunicationByMsgQueue）注册到名字服务表中。
 * =====================================================================================
 */
CStatus CMsgLoopMgrForUserDefinedQueue::Initialize()
{
	CCommunicationNameServer * pNameServer;
	try
	{
		pNameServer = CCommunicationNameServer::GetInstance();
	}
	catch(...)
	{
		return CStatus(-1,0,"in Initialize of CMsgLoopMgrForUserDefinedQueue : GetInstance of CCommunicationNameServer failed");
	}

	CStatus s1 = pNameServer->Register(m_strThreadName.c_str(),new CCommunicationByMsgQueue(m_pQueue));
	if(!s1.IsSuccess())
	{
		return s1;
	}
	return CStatus(0,0);
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CMsgLoopMgrForUserDefinedQueue
 *  Description:  在名字服务中释放一次通信接口
 * =====================================================================================
 */

CStatus CMsgLoopMgrForUserDefinedQueue:: Uninitialize()
{
 	CCommunicationNameServer * pNameServer;
 	try
	{
		pNameServer = CCommunicationNameServer::GetInstance();
	}
	catch(...)
	{
		return CStatus(-1,0,"in Uninitialize of CMsgLoopMgrForUserDefinedQueue : GetInstance of CCommunicationNameServer failed");
	}
	
	return pNameServer->ReleaseCommunicationObject(m_strThreadName.c_str());
}

