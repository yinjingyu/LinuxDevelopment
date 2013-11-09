#ifndef CEVENT_H
#define CEVENT_H

#include "CStatus.h"
#include "CConditionVariable.h"
#include "CMutex.h"
#include "CSharedEventPool.h"
class CEvent
{
public:

	CEvent();
	CEvent(bool bSemaphore);
	CEvent(const char * pstrEventName);
	CEvent(const char * pstrEventName, bool bSemaphore);
	~CEvent();

	CStatus Set();
	CStatus Wait();

private:

	//声明条件变量，条件变量的具体实现要根据CEvent的参数类型来决定
	CConditionVariable m_Cond;

	//声明互斥量，互斥量类型根据CEvent的参数来决定是采用共享型还是非
	//共享型
	CMutex m_Mutex;

	//事件信息
	SEventInfo * m_pEventInfo;

	//如果事件信息要在类内部分配，则需要被销毁
	bool m_bNeedDestroy;

	//事件名称
	string m_strEventName;
};

#endif
