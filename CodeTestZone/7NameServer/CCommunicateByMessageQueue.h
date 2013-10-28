

#ifndef CCOMMUNICATEBYMESSAGEQUEUE_H
#define CCOMMUNICATEBYMESSAGEQUEUE_H

class CCommunicateByMessageQueue : public ICommunicationObject 
{
	private:

	CMessageQueueByUserDefined * m_pMsgQueue;

	public:

	CCommunicateByMessageQueue(CMessageQueueByUserDefined * pMsgQueue);
	virtual ~CCommunicateByMessageQueue();

	virtual CStatus PostMessage(CMessage * pMsg);
};

#endif
