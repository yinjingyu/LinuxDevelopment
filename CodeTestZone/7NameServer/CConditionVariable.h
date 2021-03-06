


#ifndef CCONDITIONVARIABLE_H
#define CCONDITIONVARIABLE_H

#include "CStatus.h"
#include "CMutex.h"
#include <pthread.h>

class CConditionVariable
{
	private:
	//Linux库提供的条件变量    
	pthread_cond_t m_Cond;

	public:

	CConditionVariable();
	virtual ~CConditionVariable();

	//调用wait操作时，使用的互斥量必须由外界传入
	//如果设置成成员变量，那么每个条件变量实例对象
	//都会有一个独立的互斥量，这样起步到
	CStatus Wait(CMutex * pMutex);
	CStatus Wakeup();
	CStatus WakeupAll();

};

#endif
