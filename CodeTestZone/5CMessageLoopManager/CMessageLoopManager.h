
#ifndef CMESSAGELOOPMANAGER_H
#define CMESSAGELOOPMANAGER_H

#include "CMessage.h"
#include "CStatus.h"
#define WaitForMessageFailedTimes 5

/*
 * =====================================================================================
 *        Class:  CMessageLoopManager
 *  Description:  用于接收消息并分发消息的消息管理类
 *  			  它的主要职责就是阻塞的从消息队列中获取一个消息，
 *  			  然后根据不同的消息发送给不同的处理对象去处理消息
 *  			  这个LoopManger实际上是一个接口，它封装了一个变化点：
 *  			  消息循环机制基于的消息队列可以有多种实现：使用STL库提供的队列、自定义队列
 *  			  、管道、网络通信等等
 *  			  不同的队列实现机制，WaitForMessage DispatchMessage 以及Initialize/Unitialize
 *  			  实现的方式都不同，所以用纯虚函数来实现
 * =====================================================================================
 */
class CMessageLoopManager
{
	public:
	
	//开始进入消息循环 while(true)
	virtual CStatus EnterMessageLoop(void * pContext);
	
	protected:
	
	virtual CStatus Initialize()=0;
	virtual CStatus Uninitialize()=0;

	virtual CMessage * WaitForMessage() = 0;
	virtual CStatus DispatchMessage(CMessage * pMsg) = 0;
};


#endif
