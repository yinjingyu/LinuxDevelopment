

#ifndef CTHREADFORMSGLOOP_H
#define CTHREADFORMSGLOOP_H

#include "CMessageObserver.h"

class CThreadInitialFinishedNotifier;
class CThread;
typedef struct
{
	void * pContext;
 	CThreadInitialFinishedNotifier *  pNotifier;
}SInitialParameter;

class CThreadForMsgLoop
{
	public:
	
	CThreadForMsgLoop(const char * strThreadName, CMessageObserver * pMsgObserver);
	CThreadForMsgLoop(const char * strThreadNaem,CMessageObserver * pMsgObserver,bool bWaitForDeath);

	virtual ~CThreadForMsgLoop();
	
	CStatus Run(void * pContext);

	private:

	CThread * m_pThread;
	bool m_bWaitForDeath;
};

#endif
