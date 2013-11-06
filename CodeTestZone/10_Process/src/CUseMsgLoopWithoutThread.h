


#ifndef CUSEMSGLOOPWITHOUTTHREAD_H
#define CUSEMSGLOOPWITHOUTTHREAD_H

#include "CMsgObserver.h"
#include "CStatus.h"
#include "IUsrBizForExecObj.h"

class CUseMsgLoopWithoutThread
{
	private:

	IUsrBizForExecObj * m_pUsrBiz;

	public:

	CUseMsgLoopWithoutThread(const char * strThreadName, CMsgObserver * pMsgObserver);
	virtual ~CUseMsgLoopWithoutThread();

	CStatus Run(void * pContext);
};

#endif
