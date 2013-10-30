


#ifndef CCLIENTBUSINESSUSINGMSGLOOP_H
#define CCLIENTBUSINESSUSINGMSGLOOP_H

#include "IUsrBizForExecObj.h"
#include "CMsgLoopManager.h"

class CUsrBizUsingMsgLoop : public CUsrBizForExecObj
{
	private :
	
	CMsgLoopManager * m_pMsgLoopManager;

	public:

	CUsrBizUsingMsgLoop(CMsgLoopManager * pMsgLoopManager);
	virtual ~CUsrBizUsingMsgLoop();

	virtual CStatus RunClientBusiness(void * pContext);
};

#endif
