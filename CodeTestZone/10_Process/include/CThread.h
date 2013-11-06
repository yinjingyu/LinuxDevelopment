
#ifndef CTHREAD_H
#define CTHREAD_H

/*
 * =====================================================================================
 *        Class:  CThread
 *  Description:  用线程来实现执行体
 * =====================================================================================
 */
#include "CStatus.h"
#include "CExecutiveObject.h"
#include "CEvent.h"

class CThread : public CExecutiveObject
{
	public:

	explicit CThread(IUsrBizForExecObj * pUsrBizForExecObj);
	explicit CThread(IUsrBizForExecObj * pUsrBizForExecObj,bool bWaitForDeath);
	virtual ~CThread();
	
	//重写基类Run函数，用于：创建并执行线程
	virtual CStatus Run(void * pContext);
	
	//重写基类的“等待执行体死亡”函数
	virtual CStatus WaitForDeath();

	private:
	
	//线程启动后执行的函数,指向CExecutiveFunctionProvider对象
	//pthread_creat() API规定了，线程执行函数的：
	//（1）调用方式：static 调用，即执行该函数时，系统不会
	//传递this指针
	//（2）返回值类型：void *
	//（3）参数类型： void *
	static void * StartFunctionOfThread(void * pContext);

	//线程执行函数需要用到的参数
	void * m_pContext;

	//线程ID,调用pthread_creat 时需要保存新创建线程的ID
	pthread_t m_ThreadID;

	//由创建线程作为参数传入，判断创建线程是否需要等待子线程死亡
	bool m_bWaitForDeath;

	//用来判断线程是否创建成功 	
	bool m_bThreadCreated;

    //Run方法的调用者，希望在Run方法返回后，已经能确保新线程已经
	//运行起来了，所以设这一个事件对象，由子线程发送给创建线程
	//告诉创建线程，我（子线程）已经成功运行了
	CEvent m_EventForWaitingForNewThread;
	
	//新线程创建完成后，不应该立即去执行它的业务逻辑，
	//而是应该先等待创建者的通知（假设一种情况：创建线程调用完pthread_create后，瞬间子线程
	//就执行玩了业务逻辑，且把自己所在的对象给注销了，此时主线程还想调用子线程的一些状态变量
	//那么就会出错）
	//所以子线程创建完成后，必须等待创建线程允许执行业务逻辑的通知才能再继续执行
	CEvent m_EventForWaitingForOldThread;
};


#endif
























