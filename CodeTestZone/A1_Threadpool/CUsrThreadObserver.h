


#ifndef CUSRTHREADOBSERVER_H
#define CUSRTHREADOBSERVER_H

#include "./include/CMsgObserver.h"
#include "./include/CStatus.h"
#include "./include/CMsgLoopManager.h"

 
class CUsrThreadObserver : public CMsgObserver
{
	public:

	virtual CStatus Initialize(CMsgLoopManager * pMsgLoopMgr, void * pContext);

	CUsrThreadObserver(void * pContext);

	private:

	CStatus On_RequestResult(CMessage * pMsg);

	CStatus On_Quit(CMessage * pMsg);

	void * m_pContext;
};

#endif
