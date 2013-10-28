

#ifndef CMESSAGEOBSERVER_H
#define CMESSAGEOBSERVER_H

#include "CMessage.h"
#include "CStatus.h"
//#include "CMessageLoopManager.h"

class CMessageLoopManager;
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

	//所有的消息处理以函数的方式写到一个类中（观察者）
	//在观察者的初始化函数中，观察者把自己的所有消息处理函数注册到被观察对象（消息循环类）上
	virtual CStatus Initialize(CMessageLoopManager * pMsgLoopMgr,void * pContext) =0;
};


#endif
