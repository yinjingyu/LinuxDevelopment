

#ifndef CCOMMUNICATEBYMESSAGEQUEUE_H
#define CCOMMUNICATEBYMESSAGEQUEUE_H

#include "ICommunicationObject.h"
#include "CStatus.h"
#include "CUsrDefMsgQueue.h"
#include "CMessage.h"

class CCommunicationByMsgQueue : public ICommunicationObject 
{
	private:

	CUsrDefMsgQueue * m_pMsgQueue;

	public:

	CCommunicationByMsgQueue(CUsrDefMsgQueue * pMsgQueue);
	virtual ~CCommunicationByMsgQueue();

	virtual CStatus PostMessage(CMessage * pMsg);
};

#endif
