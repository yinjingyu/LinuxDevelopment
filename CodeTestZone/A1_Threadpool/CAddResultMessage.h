

#ifndef CADDRESULTMESSAGE_H
#define CADDRESULTMESSAGE_H

#include "./include/CMessage.h"

class CAddResultMessage : public CMessage
{
	public:
	CAddResultMessage(int iResult);

	public:

	const int & m_ciResult;

	private:
	int m_iResult;
};


#endif
