
#ifndef CMESSAGEQUEUEBYUSERDEFINED_H
#define CMESSAGEQUEUEBYUSERDEFINED_H

#include "CMessage.h"
#include "CStatus.h"

class CMessageQueueByUserDefined
{
	private:
	CMessage ** m_pQueueSpace;

	int m_iQueueTail;
	int m_iQueueHead;

	int m_iTotalRoom;

	private:
	CStatus EnlargeQueue();

	public:
 	bool IsFull();
	bool IsEmpty();

	CMessageQueueByUserDefined();

	virtual ~CMessageQueueByUserDefined();

	CStatus Push(CMessage * pMsg);
	
	CMessage * Pop();

};

#endif
