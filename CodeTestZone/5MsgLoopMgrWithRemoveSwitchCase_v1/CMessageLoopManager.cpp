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

#include "CMessageLoopManager.h"
#include "MessageIDTable.h"
#include <map>

CMessageLoopManager:: CMessageLoopManager(CMessageObserver * pMsgObserver)
{
	m_pMsgObserver = pMsgObserver;
}

CStatus CMessageLoopManager::EnterMessageLoop(void * pContext)
{
	
	//允许传递给线程的参数是NULL型，表示不传递参数，所以不做检查

	//1、进入消息循环前，允许消息循环做一些初始化的工作
 	CStatus s1 = Initialize();
	if(!s1.IsSuccess())
		return s1;

	//2、线程创建时，也许会给某些消息处理类通过pContext传递一些参数
	//所以编类所有的消息处理类，挨个的调用他们的初始化函数，把参数传递
	//给他们（这里我们是不会管会不会某些处理类不需要参数来初始化自己的）
	//如果不许要，直接丢弃掉参数就好
	std::map<unsigned int, CMessageObserver *>::iterator it1;
	for(it1 = m_MsgFuncMappingTable.begin(); it1 != m_MsgFuncMappingTable.end(); it1++)
	{
		it1->second->Initialize(pContext);
	}

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
		
		if(s2.m_ciReturnCode == QUIT_MESSAGE_LOOP)
			break;
	}
	
	//4、等消息循环退出后，说明消息队列的拥有线程不再处理任何发给
	//它的消息了。此时应该把该消息队列中的所有对象销毁
	std::map<unsigned int, CMessageObserver *>::iterator it2;
	for(it2 = m_MsgFuncMappingTable.begin(); it2 != m_MsgFuncMappingTable.end(); it2++)
	{
		delete it2->second;
	}
	
	//5、允许消息队列做一些收尾工作
	CStatus s3 = Uninitialize();
	if(!s3.IsSuccess())
		return s3;

	return CStatus(0,0);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Register
 *  Description:  把消息的类型ID和此类消息的处理对象注册到分发表中
 * =====================================================================================
 */
CStatus CMessageLoopManager::Register(unsigned int iMsgTypeID, CallBackFunctionOfMsgObserver pFunction)
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
CStatus CMessageLoopManager::DispatchMessage(CMessage * pMsg)
{
	if(0 == pMsg)
	{
		return CStatus(-1,0,"in DispatchMessage of CMessageLoopManager:bad paremeter ,pmsg is null");
	}

	//遍历查找消息处理函数
	std::map<unsigned int,CMessageObserver *>::iterator it;
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














