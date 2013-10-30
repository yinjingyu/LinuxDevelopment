/*
 * =====================================================================================
 *
 *       Filename:  CThread.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月17日 22时09分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pthread.h>

#include "CThread.h"

//在CThread的构造函数中，初始化从基类继承来的业务逻辑指针
CThread::CThread(CClientBusinessForExecObj * pClientBusinessForExecObj):CExecutiveObject(pClientBusinessForExecObj)
{
	m_pContext = 0;
	m_bWaitForDeath = false;
	m_bThreadCreated = false;
}

CThread::CThread(CClientBusinessForExecObj * pClientBusinessForExecObj,bool bWaitForDeath):CExecutiveObject(pClientBusinessForExecObj)
{
	m_pContext = 0;
	m_bWaitForDeath = bWaitForDeath;
	m_bThreadCreated = false;

}

CThread::~CThread()
{
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Run
 *  Description:  调用创建线程的API函数，并将线程ID号保存到类的成员变量
 *  			  以便在其他场合使用
 *   paremeters:  使用库的用户、线程类、和具体的线程，他们之间的位置关系是
 *   		      用户 --- 线程类 --- 线程
 *   		      如图，线程类作为一个中间件，
 *   		      当用户想要向线程传递参数时，是先把参数保存到线程类的成员变量中，然后
 *   		      线程在从线程类中获取，这就做到了封装，所谓封装某个对象A，就是避免用户直接
 *   		      和A交互（可能直接操作A比较复杂），而是让用户先和封装A的类交互（显然，封装过后
 *   		      用户获得的操作接口肯定比之前简单好用）
 * =====================================================================================
 */
CStatus CThread::Run(void * pContext)
{ 	
	m_pContext = pContext;
	
	//直接把当前对象的this指针传递给业务逻辑执行函数
	int r = pthread_create(&m_ThreadID,NULL,StartFunctionOfThread,this);
	if(r != 0)
	{
		return CStatus(-1,0,"error in Run of CThread: pthread_creat failed!");
	}
	
	m_bThreadCreated = true;

	if(!m_bWaitForDeath)
	{
		r = pthread_detach(m_ThreadID);
		if(r !=0 )
		{
			return CStatus(-1,0,"In CThread::Run() pthread_detach failed");
		}
	}

	return CStatus(0,0);
}

CStatus CThread::WaitForDeath()
{
 	if(!m_bWaitForDeath)
	{
		return CStatus(-1,0,"In CThread::WaitForDeath , m_bWaitForDeath is false which means WaitForDeath is not required!");
	}

	if(!m_bThreadCreated)
	{
		return CStatus(-1,0,"In CThread::WaitForDeath , m_bThreadCreated is null");
	}

	int r = pthread_join(m_ThreadID,0);
	if(r != 0)
	{
		return CStatus(-1,0,"error in CThread::WaitForDeath");
	}
	return CStatus(0,0);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  StartFunctionOfThread
 *  Description:  线程创建后执行的函数
 * =====================================================================================
 */
void * CThread::StartFunctionOfThread(void * pThis)
{
	//把参数重新转换会CThread对象的this指针，这样我们就可以访问它的数据成员
	CThread * pThreadThis = (CThread *)pThis;
	
	//CThread 继承了基类的私有成员m_pExecutiveFunctionProvider，该成员指向具体的
	//业务逻辑对象，接着我们调用业务逻辑对象的RunExecutiveFuntion接口开始执行业务
	pThreadThis->m_pClientBusinessForExecObj->RunClientBusiness(pThreadThis->m_pContext);

	//如果不需要等待子线程死亡，那么封装子线程对象的指针就不再需要了
	//另外，即使删除了子线程对象，子线程可能还是存在
	if(!m_bWaitForDeath)
	{
		delete pThreadThis;
	}

	return 0; 	
}





