

#ifndef CMESSAGEOBSERVER_H
#define CMESSAGEOBSERVER_H

#include "CMessage.h"
#include "CStatus.h"

/*
 * =====================================================================================
 *        Class:  CMessageObserver
 *  Description:  用于封装消息处理这个变换点的消息处理类
 * =====================================================================================
 */
class CMessageObserver
{
	public:

	CMessageObserver();
	virtual ~CMessageObserver();
	
	//所有的消息处理类都应该有一个自己初始化函数
	virtual CStatus Initialize(void * pContext) =0;
	//所有的消息处理类都应该有一个统一的消息处理接口
	virtual CStatus Notify(CMessage * pMsg)=0;
};


#endif
