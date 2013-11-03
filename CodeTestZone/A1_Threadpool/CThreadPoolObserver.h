
#ifndef CTHREADPOOLOBSERVER_H
#define CTHREADPOOLOBSERVER_H

#include "CAddThreadJobDoneMessage.h"
#include "CThreadPool.h"
#include "MessageIDTable.h"

class CThreadPoolObserver :public CMsgObserver
{
	public:

	virtual CStatus Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext);

	CThreadPoolObserver(CThreadPool * pThreadPool);
	
	private:

	CStatus On_JobDone(CMessage * pMsg);
	

	CStatus On_QuitLoopMessage(CMessage * pMsg);
 
	private:
	CThreadPool * m_pThreadPool;
};

#endif
