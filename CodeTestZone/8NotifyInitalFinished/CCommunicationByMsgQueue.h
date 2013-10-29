

#ifndef CCOMMUNICATEBYMESSAGEQUEUE_H
#define CCOMMUNICATEBYMESSAGEQUEUE_H

#include "ICommunicationObject.h"
#include "CStatus.h"
#include "CMessageQueueByUserDefined.h"
#include "CMessage.h"

class CCommunicationByMsgQueue : public ICommunicationObject 
{
	private:

	CMessageQueueByUserDefined * m_pMsgQueue;

	public:

	CCommunicationByMsgQueue(CMessageQueueByUserDefined * pMsgQueue);
	virtual ~CCommunicationByMsgQueue();

	virtual CStatus PostMessage(CMessage * pMsg);
};

#endif
