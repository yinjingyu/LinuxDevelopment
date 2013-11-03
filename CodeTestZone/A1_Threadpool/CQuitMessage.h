

#ifndef CQUITMESSAGE_H
#define CQUITMESSAGE_H

#include "./include/CMessage.h"
#include "MessageIDTable.h"

class CQuitMessage : public CMessage
{
public:
	CQuitMessage ();
	virtual ~CQuitMessage ();

private:
	/* data */
};

#endif
