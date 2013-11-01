

#ifndef ICOMMUNICATIONOBJECT_H
#define ICOMMUNICATIONOBJECT_H

#include "CStatus.h"
#include "CMessage.h"

class ICommunicationObject
{
	public:
	ICommunicationObject();
	virtual ~ICommunicationObject();

	virtual CStatus PostMessage(CMessage * pMsg) = 0;
};



#endif
