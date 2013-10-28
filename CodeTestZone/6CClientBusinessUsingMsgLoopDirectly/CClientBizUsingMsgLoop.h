


#ifndef CCLIENTBUSINESSUSINGMSGLOOP_H
#define CCLIENTBUSINESSUSINGMSGLOOP_H

#include "CClientBusinessForExecObj.h"
#include "CMessageLoopManager.h"

class CClientBizUsingMsgLoop : public CClientBusinessForExecObj
{
	private :
	
	CMessageLoopManager * m_pMsgLoopManager;

	public:

	CClientBizUsingMsgLoop(CMessageLoopManager * pMsgLoopManager);
	virtual ~CClientBizUsingMsgLoop();

	virtual CStatus RunClientBusiness(void * pContext);
};

#endif
