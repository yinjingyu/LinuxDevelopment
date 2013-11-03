  

#ifndef CADDTHREADJOBDONEMESSAGE_H
#define CADDTHREADJOBDONEMESSAGE_H

#include "./include/CMessage.h"
#include "MessageIDTable.h"

class CAddThreadJobDoneMessage : public CMessage
{
	public:

    CAddThreadJobDoneMessage(const char * strAddThreadID);

	virtual ~CAddThreadJobDoneMessage();
	public:
	
	char * GetAddThreadID();

	private:
	char * m_pStrAddThreadID;
};

#endif
