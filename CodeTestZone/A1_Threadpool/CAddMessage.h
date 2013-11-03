

#ifndef CADDMESSAGE_H
#define CADDMESSAGE_H
#include "./include/CMessage.h"
#include "MessageIDTable.h"
class CAddMessage : public CMessage
{
	public:
	
	CAddMessage(int opt1, int opt2);

 	public:
	
	const int & m_cOpt1;
	const int & m_cOpt2;

	private:

	int m_opt1;
	int m_opt2;
};


#endif
