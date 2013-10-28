
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

class CThread : public CExecutiveObject
{
	public:

	explicit CThread(CClientBusinessForExecObj * pClientBusinessForExecObj);
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
};


#endif
























