

#ifndef CTHREADFORMSGLOOP_H
#define CTHREADFORMSGLOOP_H

#include "CMsgObserver.h"

class CThreadInitFinishedNotifier;
class CThread;
typedef struct
{
	void * pContext;
 	CThreadInitFinishedNotifier *  pNotifier;
}SInitialParameter;

class CThreadUsingMsgLoop
{
	public:
	
	CThreadUsingMsgLoop(const char * strThreadName, CMsgObserver * pMsgObserver);
	CThreadUsingMsgLoop(const char * strThreadNaem,CMsgObserver * pMsgObserver,bool bWaitForDeath);

	virtual ~CThreadUsingMsgLoop();
	
	CStatus Run(void * pContext);

	private:

	CThread * m_pThread;
	bool m_bWaitForDeath;
};

#endif
