
#ifndef CMESSAGEQUEUEBYUSERDEFINED_H
#define CMESSAGEQUEUEBYUSERDEFINED_H

#include "CMessage.h"
#include "CStatus.h"
#include "CMutex.h"
#include "CEvent.h"

class CUsrDefMsgQueue
{
	private:
	CMessage ** m_pQueueSpace;

	int m_iQueueTail;
	int m_iQueueHead;

	int m_iTotalRoom;
	
	CMutex m_Mutex;
	CEvent m_Event;
	private:
	CStatus EnlargeQueue();

	CStatus Push(CMessage * pMsg);
	CMessage * Pop();

	public:
 	bool IsFull();
	bool IsEmpty();

	CUsrDefMsgQueue();
	virtual ~CUsrDefMsgQueue();
	
	CStatus PushMessage(CMessage * pMsg);
	CMessage * GetMessage();
};

#endif
