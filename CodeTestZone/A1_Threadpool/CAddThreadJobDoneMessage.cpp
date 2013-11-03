 /*
 * =====================================================================================
 *
 *       Filename:  CAddThreadJobDoneMessage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月02日 17时29分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

 #include "CAddThreadJobDoneMessage.h"
#include "MessageIDTable.h"
#include "./include/CMessage.h"
#include <string.h>

CAddThreadJobDoneMessage::CAddThreadJobDoneMessage(const char * strAddThreadID):CMessage(JOBDONE_THREADPOOL_MESSAGE)
{
	m_pStrAddThreadID = new char[strlen(strAddThreadID)+1];
	strcpy(m_pStrAddThreadID,strAddThreadID);
}	


CAddThreadJobDoneMessage:: ~CAddThreadJobDoneMessage()
{
	if(0 != m_pStrAddThreadID)
	{
		delete m_pStrAddThreadID;
		m_pStrAddThreadID = 0;
	}
}

char * CAddThreadJobDoneMessage::GetAddThreadID()
{
	return m_pStrAddThreadID;
}
