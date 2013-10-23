
#ifndef CEVENT_H
#define CEVENT_H

#include "CStatus.h"
#include "CConditionVariable.h"
#include "CMutex.h"

class CEvent
{
	private:

	CConditionVariable m_Cond;
	CMutex m_Mutex;
	
	int  m_iEventCounter;
	bool m_bIsRecordCondVar;

	public:

	CEvent();
	~CEvent();

	CStatus UseAsRecordCondVar(bool b);

	CStatus Set();
	CStatus Wait();

};

#endif
