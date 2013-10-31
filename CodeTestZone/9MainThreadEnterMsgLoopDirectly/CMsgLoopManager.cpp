/*
 * =====================================================================================
 *
 *       Filename:  CMessageLoopManager.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 10时59分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CMsgLoopManager.h"
#include "MessageIDTable.h"
#include <map>
#include "CThreadUsingMsgLoop.h"
#include "CThreadInitFinishedNotifier.h"
#include <unistd.h>
#include "CMsgObserver.h"
class CMsgObserver;

CMsgLoopManager:: CMsgLoopManager(CMsgObserver * pMsgObserver)
{
	m_pMsgObserver = pMsgObserver;
}

CMsgLoopManager:: ~CMsgLoopManager()
{
	if(0 != m_pMsgObserver)
	{
		delete m_pMsgObserver;
		m_pMsgObserver = 0;
	}
}


CStatus CMsgLoopManager::EnterMessageLoop(void * pContext)
{
	if(0 == pContext)
	{
		return CStatus(-1,0,"In CMessageLoopManager::EnterMessageLoop : pContext is null");
	}
	
	SInitialParameter * pInitParameter = (SInitialParameter *)pContext;

	//1、进入消息循环前，允许消息循环做一些初始化的工作
 	CStatus s1 = Initialize();
	if(!s1.IsSuccess())
	{
		//通知创建线程，子线程在进入消息循环前初始化失败
		pInitParameter->pNotifier->NotifyInitialFinished(false);
		return s1;
	}

	//在进入消息循环前，调用观察者的初始化函数
	CStatus s_mo_i = m_pMsgObserver->Initialize(this,pInitParameter->pContext);
	if(!s_mo_i.IsSuccess())
	{
		//通知创建线程，子线程中的消息观察者初始化失败
		pInitParameter->pNotifier->NotifyInitialFinished(false);
		return s_mo_i;
	}
	
	//通知主线程，子线程在进入消息循环前所有该做的初始化均成功，现在可以放心的给子线程发送消息了 	
	pInitParameter->pNotifier->NotifyInitialFinished(true);

	//3、消息队列的拥有线程owner开始进入一个死循环（即消息循环机制）
	//owner 阻塞的一直从消息队列中读取other线程发给它的消息
	//然后根据自己手中的消息分发表，将消息派送给不同的消息处理类去
	//处理消息。
	//当任何其他线程向他发送一个退出消息后，owner结束处理消息的工作j
	while(true)
	{
		int failed_counter = 0;
		CMessage * pMsg = WaitForMessage();
		if(0 == pMsg)
		{
			//如果等待消息失败超过5次，就认为消息队列出问题
			//推出消息循环
			if(failed_counter++ < WaitForMessageFailedTimes)	
					continue;
			else
				return CStatus(-1,0,"in EnterMessageLoop of CMessageLoopManager : wait for message faild");
		}
		
		CStatus s2 = DispatchMessage(pMsg);
		if(!s2.IsSuccess())
			return s2;
		//当处理完消息后，应该直接注销消息
		delete pMsg;	
		pMsg = 0;

		if(s2.m_ciReturnCode == QUIT_MESSAGE_LOOP)
			break;
	}	
	
	//5、允许消息队列做一些收尾工作
	CStatus s3 = Uninitialize();
	if(!s3.IsSuccess())
		return s3;

	return CStatus(1,0);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Register
 *  Description:  把消息的类型ID和此类消息的处理对象注册到分发表中
 * =====================================================================================
 */
CStatus CMsgLoopManager::Register(unsigned int iMsgTypeID, CallBackFunctionOfMsgObserver pFunction)
{
	if(0 == pFunction)
	{
		return CStatus(-1,0,"in Register of CMessageLoopManager : pMsgObserver is null");
	}

	m_MsgFuncMappingTable[iMsgTypeID] = pFunction;
	return CStatus(0,0);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DispatchMessage
 *  Description:  消息循环用来派送消息的函数
 *  			  具体做法是：根据消息的ID，去自己手中的分发表（map）中
 *  			  查找相应的处理类，然后调用他们的虚函数(统一的消息处理
 *  			  接口)
 * =====================================================================================
 */
CStatus CMsgLoopManager::DispatchMessage(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		return CStatus(-1,0,"in DispatchMessage of CMessageLoopManager:bad paremeter ,pmsg is null");
	}

	//遍历查找消息处理函数
	std::map<unsigned int,CallBackFunctionOfMsgObserver>::iterator it;
	it = m_MsgFuncMappingTable.find(pMsg->m_clMsgID);
	if(it == m_MsgFuncMappingTable.end())
	{
		return CStatus(-1,0,"尚未注册该消息");
	}

	CallBackFunctionOfMsgObserver pFunction = it->second;

	if(pFunction != 0)
		return (m_pMsgObserver->*pFunction)(pMsg);
	else
		return CStatus(-1,0,"该消息没有注册处理类");	
}














